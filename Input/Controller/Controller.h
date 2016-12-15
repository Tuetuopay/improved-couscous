#ifndef _INPUT_CONTROLLER_CONTROLLER_H
#define _INPUT_CONTROLLER_CONTROLLER_H

#include "Input/InputListener.h"
#include "Game/Entity.h"

namespace Input::Controller {

class Controller : public InputListener {
public:
	Controller(Entity *e);

	virtual void processMouseMotion(double x, double y, double xrel, double yrel, bool isClicking) {}
	virtual void processKeyboard   (int key, int scancode, int action, int mods) {}
	virtual void processScroll     (double dx, double dy) {}

private:
	// The controlled entity
	Entity *_e;

	// Keep track of rotations
	
};

}

#endif

