#include "GameEngine.h"
#include "GFX/RenderEngine.h"

#include <GLFW/glfw3.h>

#include <iostream>

#include "Input/InputManager.h"

namespace Game {

GameEngine::GameEngine()
 :_gameData({.dt = 0, .playerPos = glm::vec3(0, 0, 0), .yaw = 0, .pitch = 0}) {
	// Initialize the render engine
	_renderEngine = GFX::RenderEngine::instance();
	_renderEngine->setGameEngine(this);
	_renderEngine->setGameData(&_gameData);

	Input::InputManager::instance()->addListener(this);

	_running = true;
	_mouseButtonUp = true;
}

int GameEngine::setup() {
	return _renderEngine->setup();
}

void GameEngine::run() {
	double beginTime = glfwGetTime(), endTime = glfwGetTime();

	while (_running) {
		beginTime = glfwGetTime();
		_gameData.currentTime = beginTime;

		frame();
		_renderEngine->render();
		endTime = glfwGetTime();

		_gameData.dt = endTime - beginTime;
	}
}

void GameEngine::processLeftClick(double x, double y, bool buttonUp) {
}
void GameEngine::processRightClick(double x, double y, bool buttonUp) {
}
void GameEngine::processMiddleClick(double x, double y, bool buttonUp) {
}
void GameEngine::processMouseMotion(double x, double y, double xrel, double yrel, bool isClicking) {
}
void GameEngine::processScroll(double dx, double dy) {
}
void GameEngine::processKeyboard(int key, int scancode, int action, int mods) {
	switch (key) {
	case GLFW_KEY_ESCAPE:
		_running = false;
		break;
	}
}
void GameEngine::processText(std::string c) {
}

void GameEngine::frame() {
	// Update stuff
}

GameEngine::~GameEngine() {
	delete _renderEngine;
}

}

