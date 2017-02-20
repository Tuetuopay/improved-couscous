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

#include "FBO.h"

namespace GFX { namespace GL {

FBO::FBO (double screenWidth, double screenHeight,
          int renderTexture, bool depthTexture, bool compareRefToTexture)
 : _w(screenWidth), _h(screenHeight),
   _renderTexture{0}, _depthTexture(0), _renderCount(renderTexture) {
	glGenFramebuffers (1, &_bufID);
	glBindFramebuffer (GL_FRAMEBUFFER, _bufID);

	if (renderTexture) addRenderTexture(renderTexture);
	if (depthTexture)  addDepthTexture(compareRefToTexture);

	glBindTexture (GL_TEXTURE_2D, 0);
	glBindFramebuffer (GL_FRAMEBUFFER, 0);
}

void FBO::addRenderTexture(const int count) {
	_renderCount = count;

	if (_renderCount <= 0) return;
	if (_renderCount >= FBO_MAX_RENDER_TEXTURE) _renderCount = FBO_MAX_RENDER_TEXTURE;

	glGenTextures (count, _renderTexture);
	for (int i = 0; i < count; i++) {
		glBindTexture(GL_TEXTURE_2D, _renderTexture[i]);
		// No texel interpolation
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
		glTexImage2D (GL_TEXTURE_2D, 0, GL_RGBA8, _w, _h, 0, GL_BGRA, GL_UNSIGNED_BYTE, 0);
		glFramebufferTexture2D (
			GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0 + i, GL_TEXTURE_2D, _renderTexture[i], 0
		);
	}
}
void FBO::addDepthTexture(bool compareRefToTexture) {
	glGenTextures (1, &_depthTexture);
	glBindTexture(GL_TEXTURE_2D, _depthTexture);
	// No texel interpolation
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	// Clamping tex coord to [0.0, 1.0]
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
	if (compareRefToTexture)
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_COMPARE_MODE, GL_COMPARE_REF_TO_TEXTURE);
	glTexImage2D(GL_TEXTURE_2D, 0, GL_DEPTH_COMPONENT24, _w, _h, 0, GL_DEPTH_COMPONENT, GL_FLOAT, 0);
	glFramebufferTexture2D (
		GL_FRAMEBUFFER, GL_DEPTH_ATTACHMENT,  GL_TEXTURE_2D, _depthTexture, 0
	);
}

void FBO::enable() {
	glBindFramebuffer(GL_FRAMEBUFFER, _bufID);
}
void FBO::disable() {
	glBindFramebuffer(GL_FRAMEBUFFER, 0);
}

void FBO::bindRender (GLint textureUnit, const int renderNo) {
	if (renderNo < 0 || renderNo >= _renderCount) return;

	glActiveTexture(GL_TEXTURE0 + textureUnit);
	glBindTexture(GL_TEXTURE_2D, _renderTexture[renderNo]);
}
void FBO::bindRenders(GLint baseTextureUnit) {
	for (int i = 0; i < _renderCount; i++) {
		glActiveTexture(GL_TEXTURE0 + baseTextureUnit + i);
		glBindTexture(GL_TEXTURE_2D, _renderTexture[i]);
	}
}
void FBO::bindDepth (GLint textureUnit) {
	glActiveTexture(GL_TEXTURE0 + textureUnit);
	glBindTexture(GL_TEXTURE_2D, _depthTexture);
}

GLuint FBO::depthTexture() {
	return _depthTexture;
}
GLuint FBO::renderTexture(const int renderNo) {
	if (renderNo < 0 || renderNo >= _renderCount) return 0;

	return _renderTexture[renderNo];
}
GLuint FBO::frameBuffer() {
	return _bufID;
}

void FBO::bind() {
	glBindFramebuffer (GL_FRAMEBUFFER, _bufID);
}
void FBO::unbind() {
	glBindFramebuffer (GL_FRAMEBUFFER, 0);
}

FBO::~FBO () {
	glDeleteFramebuffers (1, &_bufID);
	glDeleteTextures (1, &_depthTexture);
	glDeleteTextures (_renderCount, _renderTexture);
}

}}

