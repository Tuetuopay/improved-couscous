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

#ifndef _GFX_COMPONENTS_LIGHT_H
#define _GFX_COMPONENTS_LIGHT_H

#include <glm/glm.hpp>
#include <cmath>

namespace GFX { namespace Components {

class Light {
public:
	Light() :
		_ambient(0.0, 0.0, 0.0, 1.0), _diffuse(1.0, 1.0, 1.0, 1.0), _specular(1.0, 1.0, 1.0, 1.0),
		_position(0.0, 0.0, 1.0, 0.0), _spotDirection(0.0, 0.0, -1.0), _spotExponent(0),
		_spotCutoff(180), _spotCosCutoff(-1),
		_attenuationConstant(1), _attenuationLinear(0), _attenuationQuadratic(0),
		_enabled(false)
	{}

	inline glm::vec4 ambient() { return _ambient; }
	inline glm::vec4 diffuse() { return _diffuse; }
	inline glm::vec4 specular() { return _specular; }
	inline glm::vec4 position() { return _position; }
	inline glm::vec3 spotDirection() { return _spotDirection; }
	inline float spotExponent() { return _spotExponent; }
	inline float spotCutoff() { return _spotCutoff; }
	inline float attenuationConstant() { return _attenuationConstant; }
	inline float attenuationLinear() { return _attenuationLinear; }
	inline float attenuationQuadratic() { return _attenuationQuadratic; }
	inline GLint enabled() { return _enabled; }

	inline void setAmbient(const glm::vec4 &ambient) {
		_ambient = glm::clamp(ambient, glm::vec4(0), glm::vec4(1));
	}
	inline void setDiffuse(const glm::vec4 &diffuse) {
		_diffuse = glm::clamp(diffuse, glm::vec4(0), glm::vec4(1));
	}
	inline void setSpecular(const glm::vec4 &specular) {
		_specular = glm::clamp(specular, glm::vec4(0), glm::vec4(1));
	}
	inline void setPosition(const glm::vec4 &position) {
		_position = position;
	}
	inline void setSpotDirection(const glm::vec3 &spotDirection) {
		_spotDirection = spotDirection;
	}
	inline void setSpotExponent(const float &spotExponent) {
		_spotExponent = glm::clamp(spotExponent, 0.f, 128.f);
	}
	inline void setSpotCutoff(const float &spotCutoff) {
		_spotCutoff = glm::clamp(spotCutoff, 0.f, 180.f);
		_spotCosCutoff = cosf(_spotCutoff * M_PI / 180.f);
	}
	inline void setAttenuationConstant(const float &attenuationConstant) {
		_attenuationConstant = glm::max(attenuationConstant, 0.f);
	}
	inline void setAttenuationLinear(const float &attenuationLinear) {
		_attenuationLinear = glm::max(attenuationLinear, 0.f);
	}
	inline void setAttenuationQuadratic(const float &attenuationQuadratic) {
		_attenuationQuadratic = glm::max(attenuationQuadratic, 0.f);
	}
	inline void setEnabled(const GLint &enabled) {
		_enabled = enabled;
	}

protected:
	glm::vec4 _ambient, _diffuse, _specular, _position;
	glm::vec3 _spotDirection;
	float _spotExponent, _spotCutoff, _spotCosCutoff;
	float _attenuationConstant, _attenuationLinear, _attenuationQuadratic;
	GLint _enabled;
	float __padding[2];
};

} }

#endif

