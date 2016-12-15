#ifndef _GAME_ENTITY_H
#define _GAME_ENTITY_H

#include <glm/glm.hpp>

namespace Game {

class Entity {
public:
	Entity(glm::vec3 pos = glm::vec3(), glm::vec3 vel = glm::vec3(), glm::vec3 acc = glm::vec3())
	 : _pos(pos), _vel(vel), _acc(acc) {}

	inline virtual void frame(const float &dt) {
		// Position
		_vel += _acc * dt;
		_pos += _vel * dt;

		// Rotation
		_rVel += _rAcc * dt;
		_rot += _rVel * dt;
	}

	inline virtual const glm::vec3& pos() const { return _pos; }
	inline virtual const glm::vec3& vel() const { return _vel; }
	inline virtual const glm::vec3& acc() const { return _acc; }
	inline virtual const glm::vec3& rot() const { return _rot; }
	inline virtual const glm::vec3& rvel() const { return _rVel; }
	inline virtual const glm::vec3& racc() const { return _rAcc; }

	inline virtual void setPos(const glm::vec3 &pos) { _pos = pos; }
	inline virtual void setVel(const glm::vec3 &vel) { _vel = vel; }
	inline virtual void setAcc(const glm::vec3 &acc) { _acc = acc; }
	inline virtual void setRot(const glm::vec3 &rot) { _rot = rot; }
	inline virtual void setRVel(const glm::vec3 &rVel) { _rVel = rVel; }
	inline virtual void setRAcc(const glm::vec3 &rAcc) { _rAcc = rAcc; }

protected:
	// Entity physics stuff
	glm::vec3 _pos, _vel, _acc;
	glm::vec3 _rot, _rVel, _rAcc;
};

}

#endif

