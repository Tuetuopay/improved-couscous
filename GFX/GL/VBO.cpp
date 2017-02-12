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

#include <type_traits>
#include <string.h>
#include "VBO.h"

namespace GFX { namespace GL {

VBO::VBO (const float *vertices, const float *texture, const float *colors, const float *normals,
		  int nVertices, const GLuint *indexes, int nIndexes, GLuint gltexture, GLenum drawMode,
		  int vertexSize, int colorSize, int textureSize)
	: _texture(gltexture), _bufInstance(0), _nVertex(nVertices), _nIndexes(nIndexes), _nVertData(vertexSize),
	  _nTexData(textureSize), _nColorData(colorSize), _mode(drawMode),
	  _instanceData(nullptr)
{
	_checkData();
	_isTexEnabled = _isColEnabled = _isNormEnabled = _isIndxEnabled = false;

	glGenVertexArrays(1, &_vao);
	glBindVertexArray(_vao);

	if (vertices) {
		glGenBuffers (1, &_bufVertex);
		glBindBuffer (GL_ARRAY_BUFFER, _bufVertex);
		glBufferData (GL_ARRAY_BUFFER, sizeof(float) * _nVertex * _nVertData, vertices,
		              GL_DYNAMIC_DRAW);
		glVertexAttribPointer(0, vertexSize, GL_FLOAT, GL_FALSE, 0, 0);
		glEnableVertexAttribArray(0);
	}
	if (texture && gltexture) {
		glGenBuffers (1, &_bufTexture);
		glBindBuffer (GL_ARRAY_BUFFER, _bufTexture);
		glBufferData (GL_ARRAY_BUFFER, sizeof(float) * _nVertex * _nTexData, texture,
		              GL_DYNAMIC_DRAW);
		glVertexAttribPointer(1, textureSize, GL_FLOAT, GL_FALSE, 0, 0);
		glEnableVertexAttribArray(1);
		_isTexEnabled = true;
	}
	if (colors) {
		glGenBuffers (1, &_bufColors);
		glBindBuffer (GL_ARRAY_BUFFER, _bufColors);
		glBufferData (GL_ARRAY_BUFFER, sizeof(float) * _nVertex * _nColorData, colors,
		              GL_DYNAMIC_DRAW);
		glVertexAttribPointer(2, colorSize, GL_FLOAT, GL_FALSE, 0, 0);
		glEnableVertexAttribArray(2);
		_isColEnabled = true;
	}
	if (normals) {
		glGenBuffers (1, &_bufNormals);
		glBindBuffer (GL_ARRAY_BUFFER, _bufNormals);
		glBufferData (GL_ARRAY_BUFFER, sizeof(float) * _nVertex * 3, normals, GL_DYNAMIC_DRAW);
		glVertexAttribPointer(3, 3, GL_FLOAT, GL_FALSE, 0, 0);
		glEnableVertexAttribArray(3);
		_isNormEnabled = true;
	}
	if (indexes) {
		glGenBuffers(1, &_bufIndexes);
		glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, _bufIndexes);
		glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(GLuint) * _nIndexes, indexes, GL_DYNAMIC_DRAW);
		_isIndxEnabled = true;
		glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);
	}
	glBindBuffer (GL_ARRAY_BUFFER, 0);
}

VBO::~VBO () {
	glDeleteBuffers (1, &_bufVertex);
	if (_instanceData) delete[] _instanceData;
}

void VBO::render() {
	if (_texture) {
		glActiveTexture(GL_TEXTURE0);
		glBindTexture(GL_TEXTURE_2D, _texture);
	}

	glBindVertexArray(_vao);

	if (_isIndxEnabled) {
		glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, _bufIndexes);
		if (_bufInstance) {
			glBindBuffer(GL_ARRAY_BUFFER, _bufInstance);
			glDrawElementsInstanced(_mode, _nIndexes, GL_UNSIGNED_INT, 0, _nInstance);
		}
		else
			glDrawElements(_mode, _nIndexes, GL_UNSIGNED_INT, BUFFER_OFFSET(0));
		glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);
	}
	else
		glDrawArrays (_mode, 0, _nVertex);
}

template<typename T>
void VBO::setInstanced(const std::list<T*> &data) {
	size_t elemSize = sizeof(T);
	// Generate buffer
	_nInstance = data.size();
	if (_instanceData) delete[] _instanceData;
	_instanceData = new GLubyte[_nInstance * elemSize];
	// Copy to buffer
	int offset = 0;
	for (T *d : data) {
		memcpy(_instanceData + offset * elemSize, d, elemSize);
		offset++;
	}

	setInstanced((T*)_instanceData, _nInstance);
}

template<typename T>
void VBO::setInstanced(const T* data, const size_t &count) {
	size_t elemSize = sizeof(T);
	_nInstance = count;
	_instanceData = (GLubyte*)data;

	glBindVertexArray(_vao);
	if (!_bufInstance) glGenBuffers(1, &_bufInstance);
	glBindBuffer(GL_ARRAY_BUFFER, _bufInstance);
	glBufferData(GL_ARRAY_BUFFER, elemSize * _nInstance, _instanceData, GL_DYNAMIC_DRAW);

	// A mat4 has to be sent in 4 separate locations
	int location = 1;
	if (std::is_same<T, glm::mat2>::value) location = 2;
	if (std::is_same<T, glm::mat3>::value) location = 3;
	if (std::is_same<T, glm::mat4>::value) location = 4;

	if (location != 1)
		for (int i = 0; i < location; i++) {
			glVertexAttribPointer(4 + i, location, GL_FLOAT, GL_FALSE, elemSize,
			                      BUFFER_OFFSET(sizeof(float) * location * i));
			glVertexAttribDivisor(4 + i, 1);
			glEnableVertexAttribArray(4 + i);
		}
	else {
		glVertexAttribPointer(4, 4, std::is_same<T, int>::value ? GL_INT : GL_FLOAT, GL_FALSE, 0, 0);
		glVertexAttribDivisor(4, 1);
		glEnableVertexAttribArray(4);
	}
}

template void VBO::setInstanced<glm::mat4>(const std::list<glm::mat4*> &data);
template void VBO::setInstanced<glm::mat3>(const std::list<glm::mat3*> &data);
template void VBO::setInstanced<glm::mat2>(const std::list<glm::mat2*> &data);
template void VBO::setInstanced<glm::vec4>(const std::list<glm::vec4*> &data);
template void VBO::setInstanced<glm::vec3>(const std::list<glm::vec3*> &data);
template void VBO::setInstanced<glm::vec2>(const std::list<glm::vec2*> &data);
template void VBO::setInstanced<float>(const std::list<float*> &data);
template void VBO::setInstanced<int>(const std::list<int*> &data);

template void VBO::setInstanced<glm::mat4>(const glm::mat4* data, const size_t &count);
template void VBO::setInstanced<glm::mat3>(const glm::mat3* data, const size_t &count);
template void VBO::setInstanced<glm::mat2>(const glm::mat2* data, const size_t &count);
template void VBO::setInstanced<glm::vec4>(const glm::vec4* data, const size_t &count);
template void VBO::setInstanced<glm::vec3>(const glm::vec3* data, const size_t &count);
template void VBO::setInstanced<glm::vec2>(const glm::vec2* data, const size_t &count);
template void VBO::setInstanced<float>(const float* data, const size_t &count);
template void VBO::setInstanced<int>(const int* data, const size_t &count);

void VBO::setBuffer(const int attribNo, const float *buffer, const int elementSize) {
	setBuffer(attribNo, (void*)buffer, GL_FLOAT, elementSize);
}
void VBO::setBuffer(const int attribNo, const int *buffer, const int elementSize) {
	setBuffer(attribNo, (void*)buffer, GL_INT, elementSize);
}

void VBO::setBuffer(
	const int attribNo, const void *buffer, const GLenum type, const int elementSize
) {
	auto bufPos = _buffers.find(attribNo);
	GLuint buf = 0;
	if (bufPos != _buffers.end())
		buf = bufPos->second;
	else {
		glGenBuffers(1, &buf);
		_buffers[attribNo] = buf;
	}

	glBindBuffer(GL_ARRAY_BUFFER, buf);
	glBufferData(
		GL_ARRAY_BUFFER,
		(type == GL_FLOAT ? sizeof(float) : sizeof(GLuint)) * _nVertex * elementSize,
		buffer, GL_DYNAMIC_DRAW
	);
	glVertexAttribPointer(attribNo, elementSize, type, GL_FALSE, 0, 0);
	glEnableVertexAttribArray(attribNo);
}

void VBO::setVertices(const float *vertices, const int nVertices, const int nVertData) {
	_nVertex = nVertices;
	_nVertData = nVertData;

	setBuffer(0, vertices, _nVertData);
}
void VBO::setTextures(const float *textures, const int nTexData) {
	_nTexData = nTexData;

	setBuffer(1, textures, _nTexData);
	_isTexEnabled = true;
}
void VBO::setColors(const float *colors, const int nColData) {
	_nColorData = nColData;

	setBuffer(2, colors, _nColorData);
	_isColEnabled = true;
}
void VBO::setNormals(const float *normals) {
	setBuffer(3, normals, 3);
}
void VBO::setIndexes(const int *indexes, const int nIndexes) {
	_nIndexes = nIndexes;

	if (!_bufIndexes) glGenBuffers(1, &_bufIndexes);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, _bufIndexes);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(GLuint) * _nIndexes, indexes,
	             GL_DYNAMIC_DRAW);
	_isIndxEnabled = true;
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);
}

void VBO::_checkData() {
	/* Vertex pointer: 2, 3, 4 */
	if (_nVertData < 2)  _nVertData = 2;
	if (_nVertData > 4)  _nVertData = 4;
	/* Texture pointer: 1, 2, 3, 4 */
	if (_nTexData < 1)   _nTexData = 1;
	if (_nTexData > 4)   _nTexData = 4;
	/* Color pointer: 3, 4 */
	if (_nColorData < 3) _nColorData = 3;
	if (_nColorData > 4) _nColorData = 4;
}

}}

