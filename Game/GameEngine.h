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

