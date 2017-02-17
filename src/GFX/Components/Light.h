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
		_attenuationConstant(1), _attenuationLinear(0), _attenuationQuadratic(0)
	{}

	inline virtual glm::vec4 ambient() { return _ambient; }
	inline virtual glm::vec4 diffuse() { return _diffuse; }
	inline virtual glm::vec4 specular() { return _specular; }
	inline virtual glm::vec4 position() { return _position; }
	inline virtual glm::vec3 spotDirection() { return _spotDirection; }
	inline virtual float spotExponent() { return _spotExponent; }
	inline virtual float spotCutoff() { return _spotCutoff; }
	inline virtual float attenuationConstant() { return _attenuationConstant; }
	inline virtual float attenuationLinear() { return _attenuationLinear; }
	inline virtual float attenuationQuadratic() { return _attenuationQuadratic; }

	inline virtual void setAmbient(const glm::vec4 &ambient) {
		_ambient = glm::clamp(ambient, glm::vec4(0), glm::vec4(1));
	}
	inline virtual void setDiffuse(const glm::vec4 &diffuse) {
		_diffuse = glm::clamp(diffuse, glm::vec4(0), glm::vec4(1));
	}
	inline virtual void setSpecular(const glm::vec4 &specular) {
		_specular = glm::clamp(specular, glm::vec4(0), glm::vec4(1));
	}
	inline virtual void setPosition(const glm::vec4 &position) {
		_position = position;
	}
	inline virtual void setSpotDirection(const glm::vec3 &spotDirection) {
		_spotDirection = spotDirection;
	}
	inline virtual void setSpotExponent(const float &spotExponent) {
		_spotExponent = glm::clamp(spotExponent, 0.f, 128.f);
	}
	inline virtual void setSpotCutoff(const float &spotCutoff) {
		_spotCutoff = glm::clamp(spotCutoff, 0.f, 180.f);
		_spotCosCutoff = cosf(_spotCutoff * M_PI / 180.f);
	}
	inline virtual void setAttenuationConstant(const float &attenuationConstant) {
		_attenuationConstant = glm::max(attenuationConstant, 0.f);
	}
	inline virtual void setAttenuationLinear(const float &attenuationLinear) {
		_attenuationLinear = glm::max(attenuationLinear, 0.f);
	}
	inline virtual void setAttenuationQuadratic(const float &attenuationQuadratic) {
		_attenuationQuadratic = glm::max(attenuationQuadratic, 0.f);
	}

protected:
	glm::vec4 _ambient, _diffuse, _specular, _position;
	glm::vec3 _spotDirection;
	float _spotExponent, _spotCutoff, _spotCosCutoff;
	float _attenuationConstant, _attenuationLinear, _attenuationQuadratic;
};

} }

#endif

