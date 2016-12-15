#ifndef _INPUT_CONTROLLER_FPS_CONTROLLER_H
#define _INPUT_CONTROLLER_FPS_CONTROLLER_H

#include "Input/InputManager.h"
#include "Input/InputListener.h"
#include "Game/Entities/Camera.h"

namespace Input { namespace Controller {

class FPSController : public InputListener {
public:
	FPSController(Game::Entities::Camera *e)
	 : _e(e), _yaw(0), _pitch(0), _fw(0.0), _rw(0.0), _sensitivity(4.0), _acc(3.0) {}
	~FPSController() {}

	virtual void
	processMouseMotion(double x, double y, double xrel, double yrel, bool isClicking) override;
	virtual void processKeyboard   (int key, int scancode, int action, int mods) override;

private:
	// The controlled entity
	Game::Entities::Camera *_e;

	// Rotations
	float _yaw, _pitch;

	// Directions
	glm::vec3 _forward, _right, _look;
	float _fw, _rw;

	// Controller parameters
	float _sensitivity, _acc;

	void _updateEntity();
};

} }

#endif

