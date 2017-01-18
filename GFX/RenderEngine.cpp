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
#include <locale>
#include <codecvt>
#include <list>

#include <GL/glew.h>
#include <GLFW/glfw3.h>

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtx/transform.hpp>
#include <glm/gtx/rotate_vector.hpp>

#include "GFX/GL/Texture.h"
#include "GFX/TextLabel.h"
#include "Input/InputManager.h"
#include "Input/Controller/FPSController.h"
#include "Game/Entities/Camera.h"

#define COUNT 30

namespace GFX {

RenderEngine* RenderEngine::_instance = nullptr;

double RenderEngine::_mouseX = 0.0, RenderEngine::_mouseY = 0.0;
bool RenderEngine::_isClicking = false;

RenderEngine::RenderEngine()
 : _winW(1280), _winH(720),
   _gameEngine(nullptr), _gameData(nullptr),
   _camera(glm::vec3(2.5f, 5.f, 7.5f), glm::vec3(0), glm::vec3(0, 1, 0)),
   _light(glm::vec3(7.5f, 7.5f, 7.5f), glm::vec3(0.0, -0.0, 0.0), glm::vec3(0, 1, 0), false),
   _fpsController(&_camera)
   {}

int RenderEngine::setup() {
	// GLFW
	if (!glfwInit()) {
		std::cerr << "Failed to init GLFW" << std::endl;
		return -1;
	}

	// Open Window
	glfwWindowHint(GLFW_SAMPLES, 4); // 4x antialiasing
	glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 4); // We want OpenGL 3.3
	glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 1);
	glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE); // To make MacOS happy; should not be needed
	glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE); //We don't want the old OpenGL

	if (!(_window = glfwCreateWindow(_winW, _winH, "C++Ube", NULL, NULL))) {
		std::cerr << "Failed to open window" << std::endl;
		glfwTerminate();
		return -2;
	}

	glfwMakeContextCurrent(_window);
	glewExperimental = true;

	if (glewInit() != GLEW_OK) {
		std::cout << "Failed to init glew" << std::endl;
		glfwTerminate();
		return -3;
	}

	// Fetching the actual buffer size to get the scaling
	int actualW, actualH;
	glfwGetFramebufferSize(_window, &actualW, &actualH);
	_scale = float(actualW) / _winW;
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
	_matOrtho = glm::perspective(glm::radians(45.f), 1.f, 1.f, 50.f);
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

	// VBO
	float vert[] = {0,0,0, 1,0,0, 1,1,0, 0,1,0, 0,0,0, 0,0,1, 0,1,1,
	                1,1,1, 1,0,1, 1,0,0, 0,0,1, 1,0,1, 0,0,1, 1,0,1},
	      tex[] =  {1,1, 2,1, 2,2, 1,2, 0,2, 0,3, 1,3, 2,3, 3,3, 3,2, 1,4, 2,4, 1,0, 2,0};
	GLuint idx[]  = {2,1,0, 0,3,2, 3,4,5, 5,6,3, 6,10,11, 11,7,6, 7,8,9, 9,2,7, 7,2,3, 3,6,7,
	                1,13,12, 12,0,1};
	for (int i = 0; i < 28; i++) tex[i] /= 4.f;
	_cube = new GL::VBO(vert, tex, nullptr, nullptr, 14, idx, 12 * 3, texGrass);

	// Generate instances
	std::list<glm::mat4*> matInstance;
	for (int x = -COUNT; x <= COUNT; x++)
		for (int z = -COUNT; z <= COUNT; z++) {
			int y = sin((x + z) / 2.0) * 2;
			matInstance.push_back(new glm::mat4(
				glm::translate(glm::vec3(x, y, z)) * glm::translate(glm::vec3(-0.5, -0.5, -0.5))
			));
		}
	_cube->setInstanced(matInstance);

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

	// FBO
	_fbo = new GL::FBO(1280 * _scale, 720 * _scale);
	_fboShadow = new GL::FBO(1280 * _scale, 720 * _scale);
	_fboLight = new GL::FBO(1024, 1024, false, true, true);

	/*
	glfwSetKeyCallback(_window, processKeypress);
	glfwSetCharCallback(_window, processText);
	glfwSetCursorPosCallback(_window, processMousePosition);
	glfwSetMouseButtonCallback(_window, processMouseButton);
	glfwSetScrollCallback(_window, processScroll);
	*/
	Input::InputManager *inputManager = Input::InputManager::instance();
	inputManager->listen(_window);
	inputManager->grabMouse();
	inputManager->addListener(&_fpsController);

	// TMP
	_matModel = glm::scale(glm::vec3(0.5, 0.5, 0.5)) * glm::translate(glm::vec3(-0.5, 0, -0.5));

	// Create the text renderer
	_textRenderer3D = new TextRenderer();
	_textRenderer2D = new TextRenderer();

	// Test
	std::shared_ptr<TextLabel> label(
		new TextLabel("This is a test!\nHello World", glm::vec3(0.f, 4.f, 0.f))
	);
	_textRenderer3D->addLabel(label);
	_labelFPS = std::shared_ptr<TextLabel>(
		new TextLabel("FPS = \nx,y,z = ", glm::vec3(0.f, 0.f, 0.f))
	);
	_textRenderer2D->addLabel(_labelFPS);

	return 0;
}

void RenderEngine::render() {
	// Light
	static float lightAngle = 0.f;

	lightAngle += _gameData->dt;
	_light.setPos(glm::rotate(glm::vec3(20, 20, 20), lightAngle, glm::vec3(0.0, 1.0, 0.0)));

	_labelFPS->setText("FPS = " + std::to_string(1.f / _gameData->dt) + "\nx,y,z = TODO");

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
	_fbo->bindRender(0);
	_fbo->bindDepth(1);
	_fboLight->bindDepth(2);
	_fboShadow->bindRender(3);
	_square->render();

	glDisable(GL_CULL_FACE);
	_textRenderer2D->render(_matHUD);
	glEnable(GL_CULL_FACE);

	glfwSwapBuffers(_window);
	glfwPollEvents();
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
	_cube->render();

	glDisable(GL_CULL_FACE);
	_textRenderer3D->render(_matProj * _matView);
	glEnable(GL_CULL_FACE);

#if 1==2
	for (int x = -COUNT; x <= COUNT; x++)
		for (int z = -COUNT; z <= COUNT; z++) {
			int y = sin((x + z) / 2.0) * 2;
			glm::mat4 mat = glm::translate(glm::vec3(x, y, z)) * glm::translate(glm::vec3(-0.5, -0.5, -0.5));
			_matMVP = _matProj * _matView * mat;
			shader->pushUniform("matMVP", 1, GL_FALSE, &_matMVP[0][0], 4);
			// _matMVP = biasMatrix * _matOrtho * _light.matrix() * mat;
			_matMVP = biasMatrix * _matOrtho * _light.matrix() * mat;
			shader->pushUniform("lightMVP", 1, GL_FALSE, &_matMVP[0][0], 4);

			_cube->render();
		}
#endif
#if 1==2
	for (int x = -COUNT; x <= COUNT; x++)
		for (int y = -COUNT; y <= COUNT; y++)
			for (int z = -COUNT; z <= COUNT; z++) {
				glm::mat4 mat = glm::translate(glm::vec3(x, y, z)) *
					glm::rotate(float(_gameData->currentTime + (x+y+z) / 8.f), glm::vec3(0, 1, 0)) *
					_matModel;
				_matMVP = _matProj * _matView * mat;
				shader->pushUniform("matMVP", 1, GL_FALSE, &_matMVP[0][0], 4);

				_cube->render();
			}
#endif
}

void RenderEngine::processKeypress(GLFWwindow* window, int key, int scancode, int action, int mods) {
	_instance->_gameEngine->processKeyboard(key, scancode, action, mods);
}
void RenderEngine::processText(GLFWwindow *window, unsigned int codepoint) {
	// UTF-8 <-> UTF-32 converter
	static std::wstring_convert<std::codecvt_utf8<wchar_t>, wchar_t> cvt;
	std::wstring s(1, codepoint);
	_instance->_gameEngine->processText(cvt.to_bytes(s));
}
void RenderEngine::processMousePosition(GLFWwindow *window, double xpos, double ypos) {
	_instance->_gameEngine->processMouseMotion(
		xpos, ypos, xpos - _mouseX, ypos - _mouseY, _isClicking
	);
	_mouseX = xpos; _mouseY = ypos;
}
void RenderEngine::processMouseButton(GLFWwindow *window, int button, int action, int mods) {
	_isClicking = action == GLFW_PRESS;
	switch(button) {
	case GLFW_MOUSE_BUTTON_LEFT:
		_instance->_gameEngine->processLeftClick(_mouseX, _mouseY, !_isClicking);
		break;
	case GLFW_MOUSE_BUTTON_RIGHT:
		_instance->_gameEngine->processRightClick(_mouseX, _mouseY, !_isClicking);
		break;
	case GLFW_MOUSE_BUTTON_MIDDLE:
		_instance->_gameEngine->processMiddleClick(_mouseX, _mouseY, !_isClicking);
		break;
	default: break;
	}
}
void RenderEngine::processScroll(GLFWwindow *window, double dx, double dy) {
	_instance->_gameEngine->processScroll(dx, dy);
}

RenderEngine::~RenderEngine() {
	delete _cube;
	delete _shaderShadow;
	delete _shaderSSAO;
	delete _shaderDepth;

	glfwTerminate();
}

}

