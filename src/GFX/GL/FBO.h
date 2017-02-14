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

#ifndef _FBO_H
#define _FBO_H

#include <GL/glew.h>

namespace GFX { namespace GL {

class FBO {
public:
	FBO(double screenWidth, double screenHeight,
	    bool renderTexture = true, bool depthTexture = true, bool compareRefToTexture = false);
	~FBO ();

	void addRenderTexture();
	// Set argument to true if you plan to use it as a sampler2DShadow in a shader
	void addDepthTexture(bool compareRefToTexture = false);

	void bind ();
	void unbind ();

	void enable ();
	void disable ();

	void bindRender (GLint textureUnit);
	void bindDepth (GLint textureUnit);

	GLuint depthTexture ();
	GLuint renderTexture ();
	GLuint frameBuffer ();

private:
	double _w, _h;
	GLuint _bufID, _renderTexture, _depthTexture;
};

}}

#endif /* defined(_FBO_H) */
