//
//  GFX/VBO.h
//  PokeFPS
//
//  Created by Alexis on 17/08/13.
//  Copyright (c) 2013 Tuetuopay. All rights reserved.
//

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
	//void setInstanced(const std::list<glm::mat4*> &matModel);
	//void setInstanced(const std::list<glm::vec4*> &data);
	//void setInstanced(const std::list<float*> &data);
	//void setInstanced(const std::list<int*> &data);

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
