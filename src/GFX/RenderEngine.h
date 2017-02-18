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

#ifndef _RENDER_ENGINE_H
#define _RENDER_ENGINE_H

namespace Game { class GameEngine; }

#include <glm/glm.hpp>

#include "GFX/GL/VBO.h"
#include "GFX/GL/FBO.h"
#include "GFX/GL/Shader.h"
#include "GFX/GL/UBO.h"
#include "GFX/TextRenderer.h"
#include "GFX/TextLabel.h"
#include "GFX/Window.h"
#include "GFX/Components/Light.h"

#include "Game/GameData.h"

#include "Game/Entities/Camera.h"
#include "Input/Controller/TrackballController.h"

#include "Models/Model.h"

namespace GFX {

typedef struct LightUBO {
	Components::Light lights[8];
} LightUBO;


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
	Window *_window;
	glm::mat4 _matProj, _matOrtho, _matView, _matModel, _matMVP, _matHUD;
	GFX::GL::VBO  *_square;
	Models::Model *_suzanne, *_ground;
	GFX::GL::Shader *_shaderShadow, *_shaderSSAO, *_shaderDepth, *_shaderColor;
	GFX::GL::FBO *_fbo, *_fboLight, *_fboShadow;
	GFX::GL::UBO<LightUBO> *_lights;
	// Window size
	int _winW, _winH;
	float _scale; // HIDPI scaling factor

	// History of game frames to average FPS over one second
	double _dts[60];
	int _dtsNo;

	inline void _makeMVP() { _matMVP = _matProj * _matView * _matModel; }

	Game::GameEngine *_gameEngine;
	Game::GameData   *_gameData;
	TextRenderer *_textRenderer3D, *_textRenderer2D;
	std::shared_ptr<TextLabel> _labelFPS;

	Game::Entities::Camera _camera, _light;
	Input::Controller::TrackballController _trackballController;
};

}

#endif
