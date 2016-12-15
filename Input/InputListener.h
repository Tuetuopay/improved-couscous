#ifndef _INPUT_INPUT_LISTENER_H
#define _INPUT_INPUT_LISTENER_H

#include <string>

namespace Input {

class InputManager;

class InputListener {
public:
	InputListener() {}
	~InputListener() {}

	virtual void processLeftClick  (double x, double y, bool buttonUp) {}
	virtual void processRightClick (double x, double y, bool buttonUp) {}
	virtual void processMiddleClick(double x, double y, bool buttonUp) {}
	virtual void processMouseMotion(double x, double y, double xrel, double yrel, bool isClicking) {}
	virtual void processScroll     (double dx, double dy) {}
	virtual void processKeyboard   (int key, int scancode, int action, int mods) {}
	virtual void processText       (std::string c) {}

	friend class InputManager;
};

}

#endif

