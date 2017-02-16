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

#ifndef _SHADER_H
#define _SHADER_H

#include <iostream>
#include <string>
#include <map>
#include <vector>

#include "GL.h"

namespace GFX { namespace GL {

class Shader {
public:
	/* By default, we will seek for shader files in the shaders/ folder
	 * We are assuming that Vertex shader is programName.vsh
	 *                      Fragmant shader is programName.fsh
	 */
	Shader (std::string programName);
	/* These are NOT filenames !!! These are shader GLSL code */
	Shader (char *vertexShader, char *fragmentShader);

	void addShader (std::string shaderName);
	void addShader (char *vertexShader, char *fragmentShader);

	void attachShader (std::string shaderName);
	void detachShader (std::string shaderName);
	void attachAllShaders ();
	void detachAllShaders ();

	void pushUniform (std::string param, GLint value);
	void pushUniform (std::string param, GLuint value);
	void pushUniform (std::string param, GLfloat value);
	void pushUniform (std::string param, GLint value1, GLint value2);
	void pushUniform (std::string param, GLuint value1, GLuint value2);
	void pushUniform (std::string param, GLfloat value1, GLfloat value2);
	void pushUniform (std::string param, GLint value1, GLint value2, GLint value3);
	void pushUniform (std::string param, GLuint value1, GLuint value2, GLuint value3);
	void pushUniform (std::string param, GLfloat value1, GLfloat value2, GLfloat value3);
	void pushUniform (std::string param, GLint value1, GLint value2, GLint value3, GLint value4);
	void pushUniform (std::string param, GLuint value1, GLuint value2, GLuint value3, GLuint value4);
	void pushUniform (std::string param, GLfloat value1, GLfloat value2, GLfloat value3, GLfloat value4);
	void pushUniform (std::string param, GLsizei count, GLboolean transpose, const GLfloat *value, int size = 4);

	void setUniform (std::string param, GLint value);
	void setUniform (std::string param, GLuint value);
	void setUniform (std::string param, GLfloat value);
	void setUniform (std::string param, GLint value1, GLint value2);
	void setUniform (std::string param, GLuint value1, GLuint value2);
	void setUniform (std::string param, GLfloat value1, GLfloat value2);
	void setUniform (std::string param, GLint value1, GLint value2, GLint value3);
	void setUniform (std::string param, GLuint value1, GLuint value2, GLuint value3);
	void setUniform (std::string param, GLfloat value1, GLfloat value2, GLfloat value3);
	void setUniform (std::string param, GLint value1, GLint value2, GLint value3, GLint value4);
	void setUniform (std::string param, GLuint value1, GLuint value2, GLuint value3, GLuint value4);
	void setUniform (std::string param, GLfloat value1, GLfloat value2, GLfloat value3, GLfloat value4);
	void setUniform (std::string param, GLsizei count, GLboolean transpose, const GLfloat *value, int size = 4);

	GLuint getUniform (std::string param);

	~Shader(void);

	void useShader (bool use);

private:
	/* These are NOT filenames !!! These are shader GLSL code */
	void loadShader (char *vertexShaderData, char *fragmentShaderData);
	/* Reads a text file. */
	char* _readTextFile (std::string file);
	char* _readTextFile (char *file);

	GLuint _programShader;
	std::vector<std::string> _shaderNames;
	std::map<std::string, GLuint> _vertShader, _fragShader;
	std::string _currentShaderName;

	std::map<std::string, GLuint> _uniforms;

	bool _isShaderAttached (GLuint shader);
	void _showInfoLog (GLuint shader);
};

}}

#endif
