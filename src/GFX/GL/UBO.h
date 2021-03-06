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

#ifndef _GFX_GL_UBO
#define _GFX_GL_UBO

#include <string>

#include "GL.h"
#include "GFX/GL/Shader.h"

namespace GFX { namespace GL {

template<typename T>
class UBO {
public:
	UBO(const std::string &name, const int &bindPoint = 0)
	 : _ubo(0), _bindPoint(bindPoint), _name(name) {
		glGenBuffers(1, &_ubo);
		glBindBuffer(GL_UNIFORM_BUFFER, _ubo);
		glBufferData(GL_UNIFORM_BUFFER, sizeof(data), nullptr, GL_STATIC_DRAW);
		glBindBuffer(GL_UNIFORM_BUFFER, 0);
	}

	virtual void update() const {
		glBindBuffer(GL_UNIFORM_BUFFER, _ubo);
		glBufferSubData(GL_UNIFORM_BUFFER, 0, sizeof(data), &data);
	}

	virtual void attach(const Shader *shader) const {
		glBindBuffer(GL_UNIFORM_BUFFER, _ubo);
		glBindBufferBase(GL_UNIFORM_BUFFER, _bindPoint, _ubo);

		GLuint program = shader->program();
		glUniformBlockBinding(
			program, glGetUniformBlockIndex(program, _name.c_str()), _bindPoint
		);
	}

	/**
	 * Returns the block size. This is useful to debug incorrect UBOs since this value should match
	 * sizeof(T)
	 */
	virtual GLint blockSize(const Shader *shader) const {
		glBindBuffer(GL_UNIFORM_BUFFER, _ubo);
		glBindBufferBase(GL_UNIFORM_BUFFER, _bindPoint, _ubo);

		GLuint program = shader->program();
		GLuint blockIndex = glGetUniformBlockIndex(program, _name.c_str());
		GLint blockSize;
		glGetActiveUniformBlockiv(program, blockIndex, GL_UNIFORM_BLOCK_DATA_SIZE, &blockSize);
		return blockSize;
	}

	inline virtual const std::string& name() const { return _name; }
	inline virtual const int& bindPoint() const { return _bindPoint; }

	T data;

protected:
	GLuint _ubo;
	int _bindPoint;
	std::string _name;
};

} }

#endif

