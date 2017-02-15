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

#ifndef _INPUT_CONTROLLER_TRACKBALL_CONTROLLER_H
#define _INPUT_CONTROLLER_TRACKBALL_CONTROLLER_H

#include "Input/InputManager.h"
#include "Input/InputListener.h"
#include "Game/Entities/Camera.h"

namespace Input { namespace Controller {

class TrackballController : public InputListener {
public:
	TrackballController(Game::Entities::Camera *e)
	 : _e(e), _yaw(0), _pitch(0), _sensitivity(4.0), _distance(10.0) {
		e->setRelativeTarget(false);
		// Simulate a static mouse motion to force-update the underlying camera
		processMouseMotion(0, 0, 0, 0, false);
	}
	~TrackballController() {}

	virtual void
	processMouseMotion(double x, double y, double xrel, double yrel, bool isClicking) override;
	virtual void processScroll(double dx, double dy) override;

private:
	// The controlled entity
	Game::Entities::Camera *_e;

	// Rotations
	float _yaw, _pitch;

	// Directions
	glm::vec3 _forward, _right, _look;

	// Controller parameters
	float _sensitivity, _distance;

	void _updateEntity();
};

} }

#endif


