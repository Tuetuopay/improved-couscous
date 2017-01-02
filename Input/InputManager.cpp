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

#include <GL/glew.h>
#include <GLFW/glfw3.h>

namespace Input {

InputManager* InputManager::_instance = nullptr;

InputManager::InputManager() : _win(nullptr), _mouseX(0.0), _mouseY(0.0), _isClicking(false) {}

void InputManager::listen(GLFWwindow *window) {
	if (_win) {
		glfwSetKeyCallback(_win, nullptr);
		glfwSetCharCallback(_win, nullptr);
		glfwSetCursorPosCallback(_win, nullptr);
		glfwSetMouseButtonCallback(_win, nullptr);
		glfwSetScrollCallback(_win, nullptr);
	}

	glfwSetKeyCallback(window, processKeypress);
	glfwSetCharCallback(window, processText);
	glfwSetCursorPosCallback(window, processMousePosition);
	glfwSetMouseButtonCallback(window, processMouseButton);
	glfwSetScrollCallback(window, processScroll);
	_win = window;

	// Gettingn the current mouse position as it will avoid some horrible glitches
	glfwGetCursorPos(_win, &_mouseX, &_mouseY);
}

// Static callbacks
void InputManager::processKeypress(GLFWwindow* window, int key, int scancode, int action, int mods) {
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
		_instance->processLeftClick(_instance->_mouseX, _instance->_mouseY, !_instance->_isClicking);
		break;
	case GLFW_MOUSE_BUTTON_RIGHT:
		_instance->processRightClick(_instance->_mouseX, _instance->_mouseY, !_instance->_isClicking);
		break;
	case GLFW_MOUSE_BUTTON_MIDDLE:
		_instance->processMiddleClick(_instance->_mouseX, _instance->_mouseY, !_instance->_isClicking);
		break;
	default: break;
	}
}
void InputManager::processScroll(GLFWwindow *window, double dx, double dy) {
	_instance->processScroll(dx, dy);
}

void InputManager::grabMouse() {
	glfwSetInputMode(_win, GLFW_CURSOR, GLFW_CURSOR_DISABLED);
}
void InputManager::ungrabMouse() {
	glfwSetInputMode(_win, GLFW_CURSOR, GLFW_CURSOR_NORMAL);
}

}

