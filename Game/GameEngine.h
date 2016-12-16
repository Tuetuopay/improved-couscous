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

#ifndef _GAME_GAME_ENGINE_H
#define _GAME_GAME_ENGINE_H

#include <string>

#include "GameData.h"
#include "Input/InputListener.h"

namespace GFX { class RenderEngine; }

namespace Game {

class GameEngine : private Input::InputListener {
public:
	GameEngine();
	~GameEngine();

	// Game entry point
	int setup();
	void run();

private:
	// Do actual computations here
	void frame();

	// Event management
	void processLeftClick  (double x, double y, bool buttonUp);
	void processRightClick (double x, double y, bool buttonUp);
	void processMiddleClick(double x, double y, bool buttonUp);
	void processMouseMotion(double x, double y, double xrel, double yrel, bool isClicking);
	void processScroll     (double dx, double dy);
	void processKeyboard   (int key, int scancode, int action, int mods);
	void processText       (std::string c);

	// Actual data
	GameData _gameData;

	// Ze render engine
	GFX::RenderEngine *_renderEngine;

	// Generic game data
	bool _running, _mouseButtonUp;

	// Make the RenderEngine a friend
	friend class GFX::RenderEngine;
};

}

#endif

