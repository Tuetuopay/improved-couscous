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

#include <iostream>
#ifdef OS_WIN32
#include <windows.h>
#endif

#include "Game/GameEngine.h"

#ifdef OS_WIN32
int WINAPI WinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance, LPSTR lpCmdLine, int nCmdShow) {
#else
int main() {
#endif
	Game::GameEngine gameEngine;

	int ret = gameEngine.setup();
	if (ret) return ret;

	gameEngine.run();
	return 0;

#if 1==2
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

	GLFWwindow *win = NULL;
	if (!(win = glfwCreateWindow(1280, 720, "C++Ube", NULL, NULL))) {
		std::cerr << "Failed to open window" << std::endl;
		glfwTerminate();
		return -2;
	}

	glfwMakeContextCurrent(win);
	glewExperimental = true;

	if (glewInit() != GLEW_OK) {
		std::cout << "Failed to init glew" << std::endl;
		glfwTerminate();
		return -3;
	}

	glfwSetInputMode(win, GLFW_STICKY_KEYS, GL_TRUE);

	glClearColor(0.0f, 0.0f, 0.4f, 0.0f);
	// Z buffer
	glEnable(GL_DEPTH_TEST);
	glDepthFunc(GL_LESS);
	// Backface culling
	glEnable(GL_CULL_FACE);
	glEnable(GL_TEXTURE_2D);

	// Matricies
	glm::mat4 matProj = glm::perspective(glm::radians(90.f), float(16.f / 9.f), 0.1f, 100.f),
	          matView = glm::lookAt(glm::vec3(2.5f, 5.f, 7.5f), glm::vec3(0), glm::vec3(0, 1, 0)),
	          matModel = glm::mat4(1.f),
	          matMVP = matProj * matView * matModel;

	// Texture
	GLuint texGrass = GFX::GL::textureFromTGA("textures/grass.tga");

	// VBO
	float vert[] = {0,0,0, 1,0,0, 1,1,0, 0,1,0, 0,0,0, 0,0,1, 0,1,1,
	                1,1,1, 1,0,1, 1,0,0, 0,0,1, 1,0,1, 0,0,1, 1,0,1},
	      tex[] =  {1,1, 2,1, 2,2, 1,2, 0,2, 0,3, 1,3, 2,3, 3,3, 3,2, 1,4, 2,4, 1,0, 2,0};
	GLuint idx[]  = {2,1,0, 0,3,2, 3,4,5, 5,6,3, 6,10,11, 11,7,6, 7,8,9, 9,2,7, 7,2,3, 3,6,7,
	                1,13,12, 12,0,1};
	for (int i = 0; i < 28; i++) tex[i] /= 4.f;
	GFX::GL::VBO cube(vert, tex, nullptr, nullptr, 14, idx, 12 * 3, texGrass);

	// Shader
	GFX::GL::Shader shader("color");
	shader.pushUniform("matMVP", 1, GL_FALSE, &matMVP[0][0], 4);
	shader.pushUniform("tex", 0);
	shader.useShader(true);

	bool culling = true;
	double lastTime = glfwGetTime(), currentTime;

	matModel = glm::scale(glm::vec3(0.5, 0.5, 0.5)) * glm::translate(glm::vec3(-0.5, 0, -0.5));

	do {
		currentTime = glfwGetTime();
		//double dt = currentTime - lastTime;
		lastTime = currentTime;

		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

#define COUNT 5
		for (int x = -COUNT; x <= COUNT; x++)
			for (int y = -COUNT; y <= COUNT; y++)
				for (int z = -COUNT; z <= COUNT; z++) {
					glm::mat4 mat = glm::translate(glm::vec3(x, y, z)) *
					           glm::rotate(float(currentTime + (x+y+z) / 8.f), glm::vec3(0, 1, 0)) *
							   matModel;
					matMVP = matProj * matView * mat;
					shader.pushUniform("matMVP", 1, GL_FALSE, &matMVP[0][0], 4);

					cube.render();
				}

		glfwSwapBuffers(win);
		glfwPollEvents();

		if (glfwGetKey(win, GLFW_KEY_B) == GLFW_PRESS) {
			culling = !culling;
			(culling ? glEnable : glDisable)(GL_CULL_FACE);
		}
	} while (glfwGetKey(win, GLFW_KEY_ESCAPE) != GLFW_PRESS && glfwWindowShouldClose(win) == 0);

	glfwTerminate();
	return 0;
#endif
}
