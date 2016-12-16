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

#ifndef _RENDER_ENGINE_H
#define _RENDER_ENGINE_H

namespace Game { class GameEngine; }

// GLFWWindow forward declaration
typedef struct GLFWwindow GLFWwindow;
#include <glm/glm.hpp>

#include "GFX/GL/VBO.h"
#include "GFX/GL/FBO.h"
#include "GFX/GL/Shader.h"

#include "Game/GameData.h"

#include "Game/Entities/Camera.h"
#include "Input/Controller/FPSController.h"

namespace GFX {

class RenderEngine {
	// Private contructor
	RenderEngine();
public:
	~RenderEngine();

	/**
	 * Setups the OpenGL context. Must be called once !
	 * @return whether the setup was successfull
	 */
	int setup();

	void render();

	// Singleton getter
	static inline RenderEngine* instance() {
		if (!_instance) _instance = new GFX::RenderEngine();
		return _instance;
	}

	inline void setGameEngine(Game::GameEngine *engine) { _gameEngine = engine; }
	inline void setGameData(Game::GameData *data) { _gameData = data; }

private:
	// Static instance
	static RenderEngine *_instance;

	// Internal methods
	void setup2D(double width, double height);
	void setup3D(double width, double height, bool perspective = true);
	void render2D();
	void render3D(GL::Shader *shader);

	// Internal data
	GLFWwindow *_window;
	glm::mat4 _matProj, _matOrtho, _matView, _matModel, _matMVP;
	GFX::GL::VBO *_cube, *_square;
	GFX::GL::Shader *_shader, *_shaderSSAO, *_shaderDepth;
	GFX::GL::FBO *_fbo, *_fboLight;
	// Window size
	int _winW, _winH;
	float _scale; // HIDPI scaling factor

	inline void _makeMVP() { _matMVP = _matProj * _matView * _matModel; }

	Game::GameEngine *_gameEngine;
	Game::GameData   *_gameData;

	Game::Entities::Camera _camera, _light;
	Input::Controller::FPSController _fpsController;

	static void processKeypress(GLFWwindow* window, int key, int scancode, int action, int mods);
	static void processText(GLFWwindow *window, unsigned int codepoint);
	static void processMousePosition(GLFWwindow *window, double xpos, double ypos);
	static void processMouseButton(GLFWwindow *window, int button, int action, int mods);
	static void processScroll(GLFWwindow *window, double dx, double dy);
	static double _mouseX, _mouseY;
	static bool _isClicking;
};

}

#endif
