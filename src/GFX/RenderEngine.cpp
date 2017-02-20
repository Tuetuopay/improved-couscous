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

#include "RenderEngine.h"
#include "Game/GameEngine.h"

#include <string>
#include <list>

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtx/transform.hpp>
#include <glm/gtx/rotate_vector.hpp>

#include "GFX/GL/Texture.h"
#include "GFX/TextLabel.h"
#include "Input/InputManager.h"
#include "Input/Controller/FPSController.h"
#include "Game/Entities/Camera.h"

#define COUNT 1

namespace GFX {

RenderEngine* RenderEngine::_instance = nullptr;

RenderEngine::RenderEngine()
 : _winW(1280), _winH(720),
   _dts{0}, _dtsNo(0),
   _gameEngine(nullptr), _gameData(nullptr),
   _camera(glm::vec3(2.5f, 5.f, 7.5f), glm::vec3(0), glm::vec3(0, 1, 0)),
   _light(glm::vec3(7.5f, 7.5f, 7.5f), glm::vec3(0.0, -0.0, 0.0), glm::vec3(0, 1, 0), false),
   _trackballController(&_camera)
   {}

int RenderEngine::setup() {
	_window = new Window("C++Ube", _winW, _winH);
	if (!_window->internalWindow()) {
		std::cout << "Window is null!" << std::endl;
		return -1;
	}
	_scale = float(_window->physicalW()) / _winW;
	std::cout << "scale = " << _scale << std::endl;

	glClearColor(0.0f, 0.0f, 0.4f, 1.0f);
	// Z buffer
	glEnable(GL_DEPTH_TEST);
	glDepthFunc(GL_LESS);
	// Backface culling
	glEnable(GL_CULL_FACE);
	glEnable(GL_TEXTURE_2D);

	// Matricies
	_matProj = glm::perspective(glm::radians(90.f), float(16.f / 9.f), 0.1f, 100.f);
	//_matOrtho = glm::ortho(-15.0, 15.0, -15.0, 15.0, 0.0, 50.0);
	_matOrtho = glm::perspective(glm::radians(45.f), 1.f, 1.f, 60.f);
	_matView = _camera.matrix();
	// _matView = glm::lookAt(glm::vec3(2.5f, 5.f, 7.5f), glm::vec3(0), glm::vec3(0, 1, 0));
	_matModel = glm::mat4(1.f);
	_makeMVP();
	_matHUD = glm::translate(
		glm::scale(
			glm::translate(
				glm::mat4(1.f), glm::vec3(-1.f, 1.f, 0.f)
			), glm::vec3(32.f / 1280.f, 32.f / 720.f, 1.f)
		), glm::vec3(0.f, -1.f, 0.f)
	);

	// Texture
	GLuint texGrass = GL::textureFromTGA("textures/grass.tga");

	_suzanne = new Models::Model("models/suzanne.obj");
	_ground = new Models::Model("models/ground.obj");

	// This quad is used for the offscreen rendering
	float v2[] = {-1,-1, -1,1, 1,1, 1,-1},
	      t2[] = {0,0, 0,1, 1,1, 1,0};
	//      t2[] = {0,1, 0,0, 1,0, 1,1};
	GLuint i2[] = {2,1,0, 0,3,2};
	_square = new GL::VBO(v2, t2, nullptr, nullptr, 4, i2, 6, texGrass, GL_TRIANGLES, 2);
	_square->setTexture(0);

	// Shader
	_shaderShadow = new GL::Shader("shadow");
	_shaderShadow->pushUniform("matMVP", 1, GL_FALSE, &_matMVP[0][0], 4);
	_shaderShadow->pushUniform("tex", 0);
	_shaderShadow->pushUniform("tex_shadow", 1);

	_shaderColor = new GL::Shader("color");
	_shaderColor->pushUniform("matMVP", 1, GL_FALSE, &_matMVP[0][0], 4);
	_shaderColor->pushUniform("tex", 0);

	_shaderSSAO = new GL::Shader("ssao");
	_shaderSSAO->pushUniform("tex_color", 0);
	_shaderSSAO->pushUniform("tex_depth", 1);
	_shaderSSAO->pushUniform("tex_shadow", 2);
	_shaderSSAO->pushUniform("tex_shadow_proj", 3);
	_shaderSSAO->pushUniform("camerarange", (GLfloat)0.1, (GLfloat)100.0);
	_shaderSSAO->pushUniform("screensize", (GLfloat)_winW, (GLfloat)_winH);

	_shaderDepth = new GL::Shader("depth");

	_lights = new GL::UBO<LightUBO>("lights", 0);
	Components::Light &light = _lights->data.lights[0];
	light.setDiffuse (glm::vec4(1.0, 1.0, 1.0, 1.0));
	light.setSpecular(glm::vec4(1.0, 1.0, 1.0, 1.0));
	light.setPosition(glm::vec4(0.0, 0.0, 2.0, 0.0));
	light.setAttenuationQuadratic(0.01);
	light.setEnabled(1);

	Components::Light &light2 = _lights->data.lights[1];
	light2.setDiffuse(glm::vec4(0.0, 1.0, 0.0, 1.0));
	light2.setSpecular(glm::vec4(0.0, 1.0, 0.0, 1.0));
	light2.setPosition(glm::vec4(0.0, 3.0, 3.0, 0.0));
	light2.setSpotDirection(glm::vec3(0.0, -3.0, -3.0));
	light2.setSpotExponent(1.f);
	light2.setSpotCutoff(20.f);
	light2.setAttenuationConstant(2);
	light2.setAttenuationQuadratic(0.01);
	light2.setEnabled(1);

	Components::Light &light3 = _lights->data.lights[2];
	light3.setDiffuse(glm::vec4(0.0, 0.0, 1.0, 1.0));
	light3.setSpecular(glm::vec4(0.0, 0.0, 1.0, 1.0));
	light3.setPosition(glm::vec4(0.0, 3.0, -3.0, 0.0));
	light3.setAttenuationConstant(2);
	light3.setAttenuationQuadratic(0.01);
	light3.setEnabled(1);

	_lights->update();

	_lights->attach(_shaderColor);

	// FBO
	_fbo = new GL::FBO(1280 * _scale, 720 * _scale);
	_fboShadow = new GL::FBO(1280 * _scale, 720 * _scale);
	_fboLight = new GL::FBO(1024, 1024, false, true, true);

	Input::InputManager *inputManager = Input::InputManager::instance();
	inputManager->listen(_window);
	inputManager->addListener(&_trackballController);

	// TMP
	_matModel = glm::scale(glm::vec3(0.5, 0.5, 0.5)) * glm::translate(glm::vec3(-0.5, 0, -0.5));

	// Create the text renderer
	_textRenderer3D = new TextRenderer();
	_textRenderer2D = new TextRenderer();

	// Test
	_labelFPS = std::shared_ptr<TextLabel>(
		new TextLabel("FPS = \nx,y,z = ", glm::vec3(0.f, 0.f, 0.f))
	);
	_textRenderer2D->addLabel(_labelFPS);

	return 0;
}

inline std::string toString(const glm::vec2 &v) {
	return std::to_string(v.x) + ", " + std::to_string(v.y);
}
inline std::string toString(const glm::vec3 &v) {
	return std::to_string(v.x) + ", " + std::to_string(v.y) + ", "
	       + std::to_string(v.z);
}
inline std::string toString(const glm::vec4 &v) {
	return std::to_string(v.x) + ", " + std::to_string(v.y) + ", "
	       + std::to_string(v.z) + ", " + std::to_string(v.w);
}
void RenderEngine::render() {
	// Light
	static float lightAngle = 0.f;

	lightAngle += _gameData->dt;
	_light.setPos(glm::rotate(glm::vec3(5, 5, 5), lightAngle, glm::vec3(0.0, 1.0, 0.0)));
	_lights->data.lights[0].setPosition(glm::vec4(_light.pos(), 1.f));
	_lights->update();

	// Update FPS
	_dts[_dtsNo++] = _gameData->dt;
	_dtsNo %= 60;
	double fps = 0;
	for (int i = 0; i < 60; i++) fps += _dts[i];
	fps /= 60.0;
	_labelFPS->setText(
		"FPS = " + std::to_string((int)roundf(1.f / fps)) + "\n"
		"x, y, z \t\t\t(" + toString(_camera.pos()) + ")\n"
		"dx, dy, dz \t\t(" + toString(_camera.vel()) + ")\n"
		"ddx, ddy, ddz \t(" + toString(_camera.acc()) + ")\n"
	);

	glEnable(GL_DEPTH_TEST);

	// Light depth
	_matView = _light.matrix();
	_matProj = _matOrtho;

	glViewport(0, 0, 1024, 1024);
	_fboLight->bind();
	_shaderDepth->useShader(true);
	glClear(GL_DEPTH_BUFFER_BIT);
	render3D(_shaderDepth);

	// Bind FBO
	_camera.frame(_gameData->dt);
	_matView = _camera.matrix();
	//_matView = glm::lookAt(glm::vec3(2.5f, 5.f, 7.5f), glm::vec3(0), glm::vec3(0, 1, 0));
	//_matView = _light.matrix();
	_matProj = glm::perspective(glm::radians(90.f), float(16.f / 9.f), 0.1f, 100.f);

	glViewport(0, 0, 1280 * _scale, 720 * _scale);

	_fbo->bind();
	_shaderColor->useShader(true);
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	render3D(_shaderColor);

	_fboShadow->bind();
	_shaderShadow->useShader(true);
	_fboLight->bindDepth(1);
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	render3D(_shaderShadow);

	_fbo->unbind();
	_shaderSSAO->useShader(true);
	glClear(GL_COLOR_BUFFER_BIT);
	glDisable(GL_DEPTH_TEST);
	_fbo->bindRenders(0);
	_fbo->bindDepth(1);
	_fboLight->bindDepth(2);
	_fboShadow->bindRenders(3);
	_square->render();

	glDisable(GL_CULL_FACE);
	_textRenderer2D->render(_matHUD);
	glEnable(GL_CULL_FACE);

	_window->swapBuffer();
	Input::InputManager::pollEvents();
}

void RenderEngine::render3D(GL::Shader *shader) {
	glm::mat4 biasMatrix(
		0.5, 0.0, 0.0, 0.0,
		0.0, 0.5, 0.0, 0.0,
		0.0, 0.0, 0.5, 0.0,
		0.5, 0.5, 0.5, 1.0
	);

	_matMVP = _matProj * _matView;
	shader->pushUniform("matMVP", 1, GL_FALSE, &_matMVP[0][0], 4);
	_matMVP = biasMatrix * _matOrtho * _light.matrix();
	shader->pushUniform("lightMVP", 1, GL_FALSE, &_matMVP[0][0], 4);
	shader->pushUniform("camera", _camera.pos().x, _camera.pos().y, _camera.pos().z);

	_suzanne->render();
	_ground->render();

	glDisable(GL_CULL_FACE);
	_textRenderer3D->render(_matProj * _matView);
	glEnable(GL_CULL_FACE);
}

RenderEngine::~RenderEngine() {
	delete _shaderShadow;
	delete _shaderSSAO;
	delete _shaderDepth;

	delete _suzanne;
	delete _ground;

	delete _window;
}

}

