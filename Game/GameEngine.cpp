/**
 * The MIT License (MIT)
 * Copyright (c) 2016 Tuetuopay
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

#include "GameEngine.h"
#include "GFX/RenderEngine.h"

#include <GLFW/glfw3.h>

#include <iostream>

#include "Input/InputManager.h"

namespace Game {

GameEngine::GameEngine() {
	 _gameData.dt = 0;
	 _gameData.playerPos = glm::vec3(0.0);
	 _gameData.yaw = 0;
	 _gameData.pitch = 0;
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

