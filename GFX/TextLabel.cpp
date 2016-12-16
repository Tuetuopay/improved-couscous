/**
 * The MIT License (MIT)
 * Copyright (c) 2016 Tuetuopay
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

#include <list>

#include "GFX/TextLabel.h"
#include "GFX/GL/Texture.h"

namespace GFX {

GLuint TextLabel::_texture = 0;

void TextLabel::makeVBO() {
	static const float v[] = {0,0, 0,1, 1,1, 1,0},
	                   t[] = {0,0, 0,1, 1,1, 1,0};
	static const GLuint i[] = {2,1,0, 0,3,2};

	if (_texture == 0)   _texture = GL::textureFromTGA("textures/ascii.tga");
	if (_vbo == nullptr) _vbo = std::shared_ptr<GL::VBO>(new GL::VBO(v, t, nullptr, nullptr, 4, i, 6, _texture));

	// Allocate a BIG buffer for all vectors
	// xy will be the letter position, while zw will be the texture uv displacement
	glm::vec4 *vecs = new glm::vec4[_text.size()];

	// Fill it
	glm::vec2 charPos(0.f);
	int visibleCount = 0;
	for (int i = 0; i < _text.size(); i++) {
		switch(_text[i]) {
		case '\n':
			charPos.x = 0.f;
			charPos.y += 1.f;
			break;
		case '\t':
			charPos.x = float(int(charPos.x + 4) % 4);
			break;
		default:
			vecs[visibleCount] = glm::vec4(charPos.x, charPos.y, _text[i] % 16, _text[i] / 16);
			// TODO : use actual character width
			charPos.x += 1.f;
			visibleCount++;
			break;
		}
	}

	// Call the "raw array" method
	_vbo->setInstanced(vecs, visibleCount);

	// Release the buffer
	delete[] vecs;
}

}

