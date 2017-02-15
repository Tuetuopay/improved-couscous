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

#ifndef _INPUT_INPUT_MANAGER_H
#define _INPUT_INPUT_MANAGER_H

#include <string>
#include <list>

#include "GFX/Window.h"
#include "Input/InputListener.h"

namespace Input {

class InputManager {
	InputManager();
public:
	// Sets to which window we listen for events
	void listen(GFX::Window *window);

	inline void addListener(InputListener *listener) {
		_listeners.push_back(listener);
	}

	static inline InputManager* instance() {
		if (!_instance) _instance = new InputManager();
		return _instance;
	}

	void grabMouse();
	void ungrabMouse();

	static void pollEvents();

private:
	// Holds the window we listrn to
	GFX::Window *_win;

	// Instance
	static InputManager *_instance;

	// Instance callbacks
	inline void processLeftClick  (double x, double y, bool buttonUp) {
		for (auto l : _listeners) l->processLeftClick(x, y, buttonUp);
	}
	inline void processRightClick (double x, double y, bool buttonUp) {
		for (auto l : _listeners) l->processRightClick(x, y, buttonUp);
	}
	inline void processMiddleClick(double x, double y, bool buttonUp) {
		for (auto l : _listeners) l->processMiddleClick(x, y, buttonUp);
	}
	inline void processMouseMotion(double x, double y, double xrel, double yrel, bool isClicking) {
		for (auto l : _listeners) l->processMouseMotion(x, y, xrel, yrel, isClicking);
	}
	inline void processScroll     (double dx, double dy) {
		for (auto l : _listeners) l->processScroll(dx, dy);
	}
	inline void processKeyboard   (int key, int scancode, int action, int mods) {
		for (auto l : _listeners) l->processKeyboard(key, scancode, action, mods);
	}
	inline void processText       (std::string c) {
		for (auto l : _listeners) l->processText(c);
	}

	// Listeners
	std::list<InputListener*> _listeners;

#if defined(WINDOW_BACKEND_GLFW3)
	// GLFW callbacks
	static void processKeypress(GLFWwindow* window, int key, int scancode, int action, int mods);
	static void processText(GLFWwindow *window, unsigned int codepoint);
	static void processMousePosition(GLFWwindow *window, double xpos, double ypos);
	static void processMouseButton(GLFWwindow *window, int button, int action, int mods);
	static void processScroll(GLFWwindow *window, double dx, double dy);
#elif defined(WINDOW_BACKEND_SDL2)
#endif

	double _mouseX, _mouseY;
	bool _isClicking;
};

}

#endif

