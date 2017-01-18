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

#ifndef _GFX_TEXT_LABEL_H
#define _GFX_TEXT_LABEL_H

#include <memory>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <GL/glew.h>

#include "GFX/GL/VBO.h"

namespace GFX {

class TextLabel {
public:
	TextLabel(const std::string &text, const glm::vec3 &pos = glm::vec3(),
	          const float &scale = 1.0, const glm::vec3 &rot = glm::vec3())
	 : _vbo(nullptr), _text(text), _pos(pos), _rot(rot), _scale(scale) {
		makeVBO();
	}

	// Render function
	inline void render() const { _vbo->render(); }
	inline glm::mat4 model() const {
		return glm::scale(
			glm::rotate(
				glm::rotate(
					glm::rotate(
						glm::translate(
							glm::mat4(1), _pos
						), _rot.z, glm::vec3(0.f, 0.f, 1.f)
					), _rot.y, glm::vec3(0.f, 1.f, 0.f)
				), _rot.x, glm::vec3(1.f, 0.f, 0.f)
			), glm::vec3(_scale, _scale, _scale));
	}

	// Getter/setters
	inline const std::string& text() const { return _text; }
	inline const glm::vec3& pos() const { return _pos; }
	inline const glm::vec3& rot() const { return _rot; }
	inline const float& scale() const { return _scale; }
	inline void setText(const std::string &text) { _text = text; makeVBO(); }
	inline void setPos(const glm::vec3 &pos) { _pos = pos; }
	inline void setRot(const glm::vec3 &rot) { _rot = rot; }
	inline void setScale(const float &scale) { _scale = scale; }

private:
	// VBO holding the character quad
	std::shared_ptr<GL::VBO> _vbo;

	// String of text
	std::string _text;

	// Position and rotation of the label
	glm::vec3 _pos, _rot;

	// Scale of the label
	float _scale;

	// Text texture
	static GLuint _texture;
	static glm::vec2 _fontDescriptor[256];

	// VBO creator
	void makeVBO();
};

}

#endif

