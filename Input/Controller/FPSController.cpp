#include "Input/Controller/FPSController.h"

#include <iostream>
#include <cmath>
#include <GLFW/glfw3.h>
#include <glm/gtx/rotate_vector.hpp>
#include <glm/gtx/string_cast.hpp>

namespace Input { namespace Controller {

void FPSController::processMouseMotion(double x, double y, double dx, double dy, bool isClicking) {
	_yaw -= _sensitivity * dx / 1000.0;
	_pitch -= _sensitivity * dy / 1000.0;

	_yaw = _yaw > 2.0 * M_PI ? _yaw - 2.0 * M_PI : (_yaw < 0.0 ? _yaw + 2.0*M_PI : _yaw);
	_pitch = fmax(fmin(_pitch, M_PI / 2.0), -M_PI / 2.0);

	// Update vectors
	_forward = glm::rotate(glm::vec3(1.0, 0.0, 0.0), _yaw, glm::vec3(0.0, 1.0, 0.0));
	_right = glm::vec3(-_forward.z, 0.0, _forward.x);
	_look = glm::rotate(
		glm::rotate(glm::vec3(1.0, 0.0, 0.0), _pitch, glm::vec3(0.0, 0.0, 1.0)),
		_yaw, glm::vec3(0.0, 1.0, 0.0)
	);

	_updateEntity();
}

void FPSController::processKeyboard(int key, int scancode, int action, int mods) {
	int way = action == GLFW_PRESS ? 1 : (action == GLFW_RELEASE ? -1 : 0);
	if (way == 0) return;

	switch (key) {
	case GLFW_KEY_W:
		_fw += way;
		break;
	case GLFW_KEY_A:
		_rw -= way;
		break;
	case GLFW_KEY_S:
		_fw -= way;
		break;
	case GLFW_KEY_D:
		_rw += way;
		break;
	}

	_updateEntity();
}

void FPSController::_updateEntity() {
	_e->setTarget(_look);
	_e->setUp(glm::cross(_right, _look));
	if (_fw == 0 && _rw == 0)
		_e->setCamAcc(glm::vec3(0.0));
	else
		_e->setCamAcc(glm::normalize(_look * _fw + _right * _rw) * _acc);
}

} }

