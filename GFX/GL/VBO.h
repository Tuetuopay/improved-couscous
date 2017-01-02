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

#ifndef _VBO_H
#define _VBO_H

#include <GL/glew.h>

#include <iostream>
#include <list>
#include <glm/glm.hpp>

#ifndef BUFFER_OFFSET
# define BUFFER_OFFSET(a) ((char*)NULL + (a))
#endif

namespace GFX { namespace GL {

class VBO {
public:
	VBO (const float *vertices, const float *texture, const float *colors, const float *normals,
			int nVertices, const GLuint *indexes, int nIndexes,
			GLuint gltexture = 0, GLenum drawMode = GL_TRIANGLES,
			int vertexSize = 3, int colorSize = 4, int textureSize = 2);
	~VBO ();

	/* Rendering. If forceTexturing is enabled, the VBO won't override current bound texture */
	void render ();
	void setTexture (GLuint texture) { _texture = texture; }

	template<typename T>
	void setInstanced(const std::list<T*> &data);
	template<typename T>
	void setInstanced(const T* data, const size_t &count);

private:
	GLuint _bufVertex, _bufTexture, _bufColors, _bufNormals,    /* VBOs */
	       _bufIndexes, _texture,                               /* Texture */
	       _vao, _bufInstance;
	int _nVertex, _nIndexes,                                    /* Amount of vertices to render */
		/* Amount of info per vertex (ex: amount of coordinates for texture) */
	    _nVertData, _nTexData, _nColorData,
		_nInstance;
	bool _isTexEnabled, _isColEnabled, _isNormEnabled, _isIndxEnabled;;
	GLenum _mode;
	GLubyte *_instanceData;

	/* Will check _nVertData, _nTexData and _nColorData */
	void _checkData ();
};

}}

#endif /* defined(__PokeFPS__VBO__) */
