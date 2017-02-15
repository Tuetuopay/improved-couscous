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

#include <GL/glew.h>
#include "GFX/Window.h"

#include <iostream>

namespace GFX {

Window::Window(const std::string &title, const int w, const int h)
 : _w(w), _h(h), _window(nullptr) {
#if defined(WINDOW_BACKEND_GLFW3)
	if (!glfwInit()) {
		std::cerr << "Failed to init GLFW" << std::endl;
		return;
	}

	// Open Window
	glfwWindowHint(GLFW_SAMPLES, 4); // 4x antialiasing
	glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 4); // We want OpenGL 3.3
	glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 1);
	glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE); // To make MacOS happy; should not be needed
	glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE); //We don't want the old OpenGL

	if (!(_window = glfwCreateWindow(_w, _h, title.c_str(), NULL, NULL))) {
		std::cerr << "Failed to open window" << std::endl;
		glfwTerminate();
		return;
	}

	glfwMakeContextCurrent(_window);
	glewExperimental = true;

	// Fetching the actual buffer size to get the scaling
	glfwGetFramebufferSize(_window, &_physicalW, &_physicalH);
#elif defined(WINDOW_BACKEND_SDL2)
#endif

	if (glewInit() != GLEW_OK) {
		std::cout << "Failed to init glew" << std::endl;
		glfwTerminate();
		_window = nullptr;
		return;
	}
}

}

