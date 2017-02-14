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

#ifndef _MODELS_MODEL_H
#define _MODELS_MODEL_H

#include <memory>
#include <string>
#include "GFX/GL/VBO.h"

namespace Models {

class Model {
public:
	/**
	 * Builds a new model from a file
	 * @param filename path to the file containing the model data
	 */
	Model(const std::string &filename);

	/**
	 * Well ... renders the model
	 */
	inline void render() { _vbo->render(); }

	~Model();

	std::shared_ptr<GFX::GL::VBO> vbo() { return _vbo; }

protected:
	// Actual GL model
	std::shared_ptr<GFX::GL::VBO> _vbo;
};

}

#endif // defined(_MODELS_MODEL_H)

