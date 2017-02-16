/**
 * The MIT License (MIT)
 * Copyright (c) 2016-2017 Tuetuopay
 *
 * Permission is hereby granted, free of charge, to any person obtaining a copy
 * of this software and associated documentation files (the "Software"), to
 * deal in the Software without restriction, including without limitation the
 * rights to use, copy, modify, merge, publish, distribute, sublicense, and/or
 * sell copies of the Software, and to permit persons to whom the Software is
 * furnished to do so, subject to the following conditions:
 *
 * The above copyright notice and this permission notice shall be included in
 * all copies or substantial portions of the Software.
 *
 * THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
 * IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
 * FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
 * AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
 * LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING
 * FROM, OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS
 * IN THE SOFTWARE.
 **/

#include "Input/InputManager.h"

#include <iostream>
#include <string>
#include <locale>
#include <codecvt>

namespace Input {

InputManager* InputManager::_instance = nullptr;

InputManager::InputManager() : _win(nullptr), _mouseX(0.0), _mouseY(0.0), _isClicking(false) {}

void InputManager::listen(GFX::Window *window) {
#if defined(WINDOW_BACKEND_GLFW3)
	if (_win) {
		glfwSetKeyCallback(_win->internalWindow(), nullptr);
		glfwSetCharCallback(_win->internalWindow(), nullptr);
		glfwSetCursorPosCallback(_win->internalWindow(), nullptr);
		glfwSetMouseButtonCallback(_win->internalWindow(), nullptr);
		glfwSetScrollCallback(_win->internalWindow(), nullptr);
	}

	glfwSetKeyCallback(window->internalWindow(), processKeypress);
	glfwSetCharCallback(window->internalWindow(), processText);
	glfwSetCursorPosCallback(window->internalWindow(), processMousePosition);
	glfwSetMouseButtonCallback(window->internalWindow(), processMouseButton);
	glfwSetScrollCallback(window->internalWindow(), processScroll);

	// Gettingn the current mouse position as it will avoid some horrible glitches
	glfwGetCursorPos(window->internalWindow(), &_mouseX, &_mouseY);
#elif defined(WINDOW_BACKEND_SDL2)
	SDL_SetEventFilter(processEvent, nullptr);
#endif

	_win = window;
}

#if defined(WINDOW_BACKEND_GLFW3)
// Static callbacks
void
InputManager::processKeypress(GLFWwindow* window, int key, int scancode, int action, int mods) {
	_instance->processKeyboard(key, scancode, action, mods);
}
void InputManager::processText(GLFWwindow *window, unsigned int codepoint) {
	// UTF-8 <-> UTF-32 converter
	static std::wstring_convert<std::codecvt_utf8<wchar_t>, wchar_t> cvt;
	std::wstring s(1, codepoint);
	_instance->processText(cvt.to_bytes(s));
}
void InputManager::processMousePosition(GLFWwindow *window, double xpos, double ypos) {
	_instance->processMouseMotion(
		xpos, ypos, xpos - _instance->_mouseX, ypos - _instance->_mouseY, _instance->_isClicking
	);
	_instance->_mouseX = xpos; _instance->_mouseY = ypos;
}
void InputManager::processMouseButton(GLFWwindow *window, int button, int action, int mods) {
	_instance->_isClicking = action == GLFW_PRESS;
	switch(button) {
	case GLFW_MOUSE_BUTTON_LEFT:
		_instance->processLeftClick(
			_instance->_mouseX, _instance->_mouseY, !_instance->_isClicking
		);
		break;
	case GLFW_MOUSE_BUTTON_RIGHT:
		_instance->processRightClick(
			_instance->_mouseX, _instance->_mouseY, !_instance->_isClicking
		);
		break;
	case GLFW_MOUSE_BUTTON_MIDDLE:
		_instance->processMiddleClick(
			_instance->_mouseX, _instance->_mouseY, !_instance->_isClicking
		);
		break;
	default: break;
	}
}
void InputManager::processScroll(GLFWwindow *window, double dx, double dy) {
	_instance->processScroll(dx, dy);
}

#elif defined(WINDOW_BACKEND_SDL2)

int InputManager::processEvent(void *userdata, SDL_Event *event) {
	switch (event->type) {
		// Clicks
	case SDL_MOUSEBUTTONUP:
	case SDL_MOUSEBUTTONDOWN:
		_instance->_mouseX = event->button.x;
		_instance->_mouseY = event->button.y;
		_instance->_isClicking = event->button.state == SDL_PRESSED;
		switch (event->button.button) {
		case SDL_BUTTON_LEFT:
			_instance->processLeftClick(
				event->button.x, event->button.y, event->button.state == SDL_RELEASED
			);
			break;
		case SDL_BUTTON_RIGHT:
			_instance->processRightClick(
				event->button.x, event->button.y, event->button.state == SDL_RELEASED
			);
			break;
		case SDL_BUTTON_MIDDLE:
			_instance->processMiddleClick(
				event->button.x, event->button.y, event->button.state == SDL_RELEASED
			);
			break;
		}
		break;

	case SDL_MOUSEMOTION:
		_instance->_mouseX = event->motion.x;
		_instance->_mouseY = event->motion.y;
		_instance->processMouseMotion(
			event->motion.x, event->motion.y,
			event->motion.xrel, event->motion.yrel,
			_instance->_isClicking
		);
		break;

	case SDL_MOUSEWHEEL:
		_instance->processScroll(double(event->wheel.x) / 10.0, double(event->wheel.y) / 10.0);
		break;

	case SDL_KEYUP:
	case SDL_KEYDOWN:
		_instance->processKeyboard(
			event->key.keysym.scancode, event->key.keysym.sym,
			event->key.state, event->key.keysym.mod
		);
		break;

	case SDL_TEXTINPUT:
		_instance->processText(event->text.text);
		break;

	default:
		return 0;
	}

	return 1;
}

#endif

void InputManager::grabMouse() {
#if defined(WINDOW_BACKEND_GLFW3)
	glfwSetInputMode(_win->internalWindow(), GLFW_CURSOR, GLFW_CURSOR_DISABLED);
#elif defined(WINDOW_BACKEND_SDL2)
#endif
}
void InputManager::ungrabMouse() {
#if defined(WINDOW_BACKEND_GLFW3)
	glfwSetInputMode(_win->internalWindow(), GLFW_CURSOR, GLFW_CURSOR_NORMAL);
#elif defined(WINDOW_BACKEND_SDL2)
#endif
}

void InputManager::pollEvents() {
#if defined(WINDOW_BACKEND_GLFW3)
	glfwPollEvents();
#elif defined(WINDOW_BACKEND_SDL2)
	SDL_PumpEvents();
#endif
}

}

