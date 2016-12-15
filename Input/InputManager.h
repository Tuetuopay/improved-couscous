#ifndef _INPUT_INPUT_MANAGER_H
#define _INPUT_INPUT_MANAGER_H

typedef struct GLFWwindow GLFWwindow;
#include <string>
#include <list>

#include "Input/InputListener.h"

namespace Input {

class InputManager {
	InputManager();
public:
	// Sets to which window we listen for events
	void listen(GLFWwindow *window);

	inline void addListener(InputListener *listener) {
		_listeners.push_back(listener);
	}

	static inline InputManager* instance() {
		if (!_instance) _instance = new InputManager();
		return _instance;
	}

	void grabMouse();
	void ungrabMouse();

private:
	// Holds the window we listrn to
	GLFWwindow *_win;

	// Instance
	static InputManager *_instance;

	// Instance callbacks
	inline void processLeftClick  (double x, double y, bool buttonUp) {
		for (auto l : _listeners) l->processLeftClick(x, y, buttonUp);
	}
	inline void processRightClick (double x, double y, bool buttonUp) {
		for (auto l : _listeners) l->processRightClick(x, y, buttonUp);
	}
	inline void processMiddleClick(double x, double y, bool buttonUp) {
		for (auto l : _listeners) l->processMiddleClick(x, y, buttonUp);
	}
	inline void processMouseMotion(double x, double y, double xrel, double yrel, bool isClicking) {
		for (auto l : _listeners) l->processMouseMotion(x, y, xrel, yrel, isClicking);
	}
	inline void processScroll     (double dx, double dy) {
		for (auto l : _listeners) l->processScroll(dx, dy);
	}
	inline void processKeyboard   (int key, int scancode, int action, int mods) {
		for (auto l : _listeners) l->processKeyboard(key, scancode, action, mods);
	}
	inline void processText       (std::string c) {
		for (auto l : _listeners) l->processText(c);
	}

	// Listeners
	std::list<InputListener*> _listeners;

	// GLFW callbacks
	static void processKeypress(GLFWwindow* window, int key, int scancode, int action, int mods);
	static void processText(GLFWwindow *window, unsigned int codepoint);
	static void processMousePosition(GLFWwindow *window, double xpos, double ypos);
	static void processMouseButton(GLFWwindow *window, int button, int action, int mods);
	static void processScroll(GLFWwindow *window, double dx, double dy);
	double _mouseX, _mouseY;
	bool _isClicking;
};

}

#endif

