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

#ifndef _INPUT_CALLBACK_LISTENER
#define _INPUT_CALLBACK_LISTENER

#include <string>
#include <function>
#include <list>

#include "Input/InputListener.h"

namespace Input {

class CallbackListener : public InputListener {
public:
	CallbackListener() {};

	inline virtual void processLeftClick  (double x, double y, bool buttonUp) const {
		for (auto &f : _leftClicks) f(x, y, buttonUp);
	}
	inline virtual void processRightClick (double x, double y, bool buttonUp) const {
		for (auto &f : _rightClicks) f(x, y, buttonUp);
	}
	inline virtual void processMiddleClick(double x, double y, bool buttonUp) const {
		for (auto &f : _middleClicks) f(x, y, buttonUp);
	}
	inline virtual void
	processMouseMotion(double x, double y, double xrel, double yrel, bool isClicking) const {
		for (auto &f : _motions) f(x, y, xrel, yrel, isClicking);
	}
	inline virtual void processScroll     (double dx, double dy) const {
		for (auto &f : _scrolls) f(dx, dy);
	}
	inline virtual void processKeyboard   (int key, int scancode, int action, int mods) const {
		for (auto &f : _keys) f(key, scancode, action, mods);
	}
	inline virtual void processText       (std::string c) const {
		for (auto &f : _texts) f(c);
	}

	inline virtual void onLeftClick  (std::function<void(double, double, bool)> f) const {
		_leftClicks.push_back(f);
	}
	inline virtual void onRightClick (std::function<void(double, double, bool)> f) const {
		_rightClicks.push_back(f);
	}
	inline virtual void onMiddleClick(std::function<void(double, double, bool)> f) const {
		_middleClicks.push_back(f);
	}
	inline virtual void
	onMouseMotion(std::function<void(double, double, double, double, bool)> f) const {
		_motions.push_back(f);
	}
	inline virtual void onScroll     (std::function<void(double, double)> f) const {
		_scrolls.push_back(f);
	}
	inline virtual void onKeyboard   (std::function<void(int, int, int, int)> f) const {
		_keys.push_back(f);
	}
	inline virtual void onText       (std::function<void(std::string)> f) const {
		_texts.push_back(f);
	}

private:
	std::list<std::function<void(double, double, bool)> _leftClicks, _rightClicks, _middleClicks;
	std::list<std::function<void(double, double, double, double, bool)> _motions;
	std::list<std::function<void(double, double)> _scrolls;
	std::list<std::function<void(int, int, int, int)> _keys;
	std::list<std::function<void(std::string)> _texts;
};

}

#endif

