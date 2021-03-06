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

#include "Input/Controller/FPSController.h"

#include <iostream>
#include <cmath>
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
	int way = action == HID_PRESS ? 1 : (action == HID_RELEASE ? -1 : 0);
	if (way == 0) return;

	switch (key) {
	case HID_KEY_W:
		_fw += way;
		break;
	case HID_KEY_A:
		_rw -= way;
		break;
	case HID_KEY_S:
		_fw -= way;
		break;
	case HID_KEY_D:
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

