//
//  FBO.h
//  PokeFPS
//
//  Created by Alexis on 19/09/13.
//  Copyright (c) 2013 Tuetuopay. All rights reserved.
//

#ifndef _FBO_H
#define _FBO_H

#include <GL/glew.h>

namespace GFX { namespace GL {

class FBO
{
public:
	FBO(double screenWidth, double screenHeight, bool renderTexture = true, bool depthTexture = true);
	~FBO ();

	void addRenderTexture();
	void addDepthTexture();

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
