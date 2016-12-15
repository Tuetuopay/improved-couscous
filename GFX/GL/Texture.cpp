#include "Texture.h"

namespace GFX { namespace GL {

GLuint textureFromTGA(std::shared_ptr<GFX::TGA> tga) {
	GLuint tex;

	// Generate texture
	glGenTextures(1, &tex);
	if (tex == 0) return tex;

	glBindTexture(GL_TEXTURE_2D, tex);
	glTexImage2D(GL_TEXTURE_2D, 0, tga->bpp == 24 ? GL_RGB : GL_RGBA, tga->w, tga->h,
	             0, tga->bpp == 24 ? GL_BGR : GL_BGRA, GL_UNSIGNED_BYTE, tga->data.get());
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);
	glGenerateMipmap(GL_TEXTURE_2D);

	return tex;
}

GLuint textureFromTGA(const std::string &filename) {
	std::shared_ptr<GFX::TGA> tga = GFX::loadTGA(filename);

	if (tga == nullptr) return 0;
	return textureFromTGA(tga);
}

}}

