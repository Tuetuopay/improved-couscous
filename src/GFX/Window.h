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

#ifndef _GFX_WINDOW_H
#define _GFX_WINDOW_H

#include <string>

#if defined(WINDOW_BACKEND_GLFW3)
# include <GLFW/glfw3.h>

// Alias for the internal window
typedef GLFWwindow InternalWindow;

#elif defined(WINDOW_BACKEND_SDL2)
# include <SDL2/SDL.h>
# error "SDL2 backend not yet implemented"
#else
# error "Unknown backend or no windowing backend selected"
#endif

namespace GFX {

class Window {
public:
	/**
	 * Builds a new window with the given size
	 * @param title text for the window's title
	 * @param w     window's width
	 * @param h     window's height
	 */
	Window(const std::string &title, const int width, const int height);

	~Window();

	/**
	 * Accesses the logical size of the window in points
	 */
	inline virtual int w() const { return _w; }
	inline virtual int h() const { return _h; }

	/**
	 * Accesses the physical size of the window in pixels instead of points
	 */
	inline virtual int physicalW() const { return _physicalW; }
	inline virtual int physicalH() const { return _physicalH; }

	/**
	 * Get the underlying raw window pointer
	 */
	inline virtual InternalWindow* internalWindow() { return _window; }

	/**
	 * Swaps the displayed buffer with the current OpenGL buffer
	 */
	inline virtual void swapBuffer() const {
#if defined(WINDOW_BACKEND_GLFW3)
		glfwSwapBuffers(_window);
#elif defined(WINDOW_BACKEND_SDL2)
#endif
	}

protected:
	// Window's size
	int _w, _h,
	    _physicalW, _physicalH;

	// Underlying native window
	InternalWindow *_window;
};

}

#endif

