#ifndef _TEXTURE_H
#define _TEXTURE_H

#include <GL/glew.h>
#include <memory>

#include "GFX/TGA.h"

namespace GFX { namespace GL {

GLuint textureFromTGA(std::shared_ptr<GFX::TGA> tga);
GLuint textureFromTGA(const std::string &filename);

}}

#endif

