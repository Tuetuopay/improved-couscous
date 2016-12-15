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
