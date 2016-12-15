//
//  VBO.cpp
//  PokeFPS
//
//  Created by Alexis on 17/08/13.
//  Copyright (c) 2013 Tuetuopay. All rights reserved.
//

#include <type_traits>
#include "VBO.h"

namespace GFX { namespace GL {

VBO::VBO (const float *vertices, const float *texture, const float *colors, const float *normals,
		  int nVertices, const GLuint *indexes, int nIndexes, GLuint gltexture, GLenum drawMode,
		  int vertexSize, int colorSize, int textureSize)
	: _texture(gltexture), _nVertex(nVertices), _nIndexes(nIndexes), _nVertData(vertexSize),
	  _nTexData(textureSize), _nColorData(colorSize), _mode(drawMode),
      _bufInstance(0), _instanceData(nullptr)
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

	glBindVertexArray(_vao);
	if (!_bufInstance) glGenBuffers(1, &_bufInstance);
	glBindBuffer(GL_ARRAY_BUFFER, _bufInstance);
	glBufferData(GL_ARRAY_BUFFER, elemSize * _nInstance, _instanceData, GL_DYNAMIC_DRAW);

	// A mat4 has to be sent in 4 separate locations
	if (std::is_same<T, glm::mat4>::value)
		for (int i = 0; i < 4; i++) {
			printf("%d\n", i);
			glVertexAttribPointer(4 + i, 4, GL_FLOAT, GL_FALSE, elemSize, BUFFER_OFFSET(sizeof(glm::vec4) * i));
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
template void VBO::setInstanced<glm::vec4>(const std::list<glm::vec4*> &data);
template void VBO::setInstanced<float>(const std::list<float*> &data);
template void VBO::setInstanced<int>(const std::list<int*> &data);

void VBO::_checkData()	{
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

