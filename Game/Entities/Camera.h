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

#ifndef _GAME_ENTITIES_CAMERA_H
#define _GAME_ENTITIES_CAMERA_H

#include <iostream>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtx/transform.hpp>
#include <glm/gtx/string_cast.hpp>

#include "Game/Entity.h"

namespace Game { namespace Entities {

class Camera : public Entity {
public:
	Camera(const glm::vec3 &pos,
	       const glm::vec3 &target,
	       const glm::vec3 &up,
	       const bool relativeTarget = true)
	 : Entity(pos), _target(target), _up(up), _relativeTarget(relativeTarget) {}

	inline virtual glm::mat4 matrix() const {
		return glm::lookAt(
			_pos,
			_relativeTarget ? _pos + _target : _target,
			_up
		);
	}

	inline virtual void lookAt(
		const glm::vec3 &pos,
		const glm::vec3 &target,
		const glm::vec3 &up = glm::vec3(0, 1, 0)
	) {
		_pos = pos;
		_target = target;
		_up = up;
	}

	inline virtual void frame(const float &dt) override {
		_vel += _camAcc * dt;
		_vel -= _vel * dt;
		Entity::frame(dt);
	}

	inline virtual void setTarget(const glm::vec3 &target) { _target = target; }
	inline virtual void setUp(const glm::vec3 &up) { _up = up; }
	inline virtual void setCamAcc(const glm::vec3 &acc) { _camAcc = acc; }

	inline virtual void print(std::ostream &out) const {
		out << "p(" << glm::to_string(_pos) << ")), t(" << glm::to_string(_target) << ")";
	}

protected:
	glm::vec3 _target, _up, _camAcc;
	bool _relativeTarget;
};

} }

#endif
