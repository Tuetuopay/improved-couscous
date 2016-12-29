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

#include "Shader.h"

namespace GFX { namespace GL {

Shader::Shader (std::string programName)
 : _programShader(0), _vertShader(), _fragShader(), _currentShaderName(programName) {
	programName = "shaders/" + programName;

	loadShader (_readTextFile (programName + ".vsh"), _readTextFile (programName + ".fsh"));
}

Shader::Shader (char *vertexShader, char *fragmentShader) {
	_currentShaderName = "shader" + std::to_string ((long long)_vertShader.size ());
	if (vertexShader && fragmentShader)
		loadShader (vertexShader, fragmentShader);
}

Shader::~Shader(void) {
}

void Shader::addShader (std::string shaderName) {
	_currentShaderName = shaderName;
	shaderName = "shaders/" + shaderName;

	loadShader (_readTextFile (shaderName + ".vsh"), _readTextFile (shaderName + ".fsh"));
}
void Shader::addShader (char *vertexShader, char *fragmentShader) {
	_currentShaderName = "shader" + std::to_string ((long long)_vertShader.size ());
	if (vertexShader && fragmentShader)
		loadShader (vertexShader, fragmentShader);
}

char* Shader::_readTextFile (char *file) {
	return _readTextFile (std::string (file));
}
char* Shader::_readTextFile (std::string file) {
	if (!file.size ()) return NULL;
	FILE *f = fopen (file.c_str (), "rb");

	if (!f) return NULL;

	fseek (f, 0, SEEK_END);
	long len = ftell (f);
	rewind (f);

	char *buf = new char[len + 3];
	fread (buf, 1, len, f);
	buf[len] = '\0';

	fclose (f);

	return buf;
}

void Shader::_showInfoLog (GLuint shader) {
	GLint length = 0;
	GLchar *buf = NULL;

	glGetShaderiv (shader, GL_INFO_LOG_LENGTH, &length);
	buf = new GLchar[length];
	glGetShaderInfoLog (shader, length, NULL, buf);
	std::cout << buf << '\n';
}

void Shader::loadShader (char *vertexShaderData, char *fragmentShaderData) {
	GLuint vertShader = 0, fragShader = 0;
	vertShader = glCreateShader (GL_VERTEX_SHADER);
	fragShader = glCreateShader (GL_FRAGMENT_SHADER);

	const char *vshd = vertexShaderData, *fshd = fragmentShaderData;

	glShaderSource (vertShader, 1, &vshd, NULL);
	glShaderSource (fragShader, 1, &fshd, NULL);

	free (vertexShaderData);
	free (fragmentShaderData);

	glCompileShader (vertShader);
	glCompileShader (fragShader);

	GLint isOk;
	glGetShaderiv (vertShader, GL_COMPILE_STATUS, &isOk);
	if (!isOk) {
		fprintf(stdout, " *** Failed to compile vertex shader *** \n");
		_showInfoLog (vertShader);
		glDeleteShader(vertShader);
		return;
	}
	glGetShaderiv (fragShader, GL_COMPILE_STATUS, &isOk);
	if (!isOk) {
		fprintf(stdout, " *** Failed to compile fragment shader *** \n");
		_showInfoLog (fragShader);
		glDeleteShader(fragShader);
		return;
	}

	_programShader = glCreateProgram ();

	glAttachShader (_programShader, vertShader);
	glAttachShader (_programShader, fragShader);

	_shaderNames.push_back (_currentShaderName);
	_vertShader[_currentShaderName] = vertShader;
	_fragShader[_currentShaderName] = fragShader;

	glLinkProgram (_programShader);
	glUseProgram (_programShader);
}

void Shader::useShader (bool use) {
	glUseProgram ((use) ? _programShader : 0);
}

void Shader::attachShader (std::string shaderName) {
	if (_isShaderAttached (_vertShader[shaderName]) || _isShaderAttached (_fragShader[shaderName]))
		return;

	glAttachShader (_programShader, _vertShader[shaderName]);
	glAttachShader (_programShader, _fragShader[shaderName]);
}
void Shader::detachShader (std::string shaderName) {
	if (!_isShaderAttached (_vertShader[shaderName]) || !_isShaderAttached (_fragShader[shaderName]))
		return;

	glDetachShader (_programShader, _vertShader[shaderName]);
	glDetachShader (_programShader, _fragShader[shaderName]);
}
void Shader::attachAllShaders () {
	for (unsigned int i = 0; i < _vertShader.size (); i++) {
		attachShader (_shaderNames[i]);
		attachShader (_shaderNames[i]);
	}
}
void Shader::detachAllShaders () {
	for (unsigned int i = 0; i < _vertShader.size (); i++) {
		detachShader (_shaderNames[i]);
		detachShader (_shaderNames[i]);
	}
}

bool Shader::_isShaderAttached (GLuint shader) {
	GLsizei count;
	GLuint *shaders = new GLuint[_vertShader.size()];
	bool ret = false;

	glGetAttachedShaders (_programShader, (int)_vertShader.size () + 3, &count, shaders);

	for (int i = 0; i < count; i++)
		if (shaders[i] == shader)
			ret = true;
	delete[] shaders;
	return ret;
}

void Shader::pushUniform (std::string param, GLint value) {
	_uniforms[param] = glGetUniformLocation (_programShader, param.c_str ());
	glUniform1i (_uniforms[param], value);
}
void Shader::pushUniform (std::string param, GLuint value) {
	_uniforms[param] = glGetUniformLocation (_programShader, param.c_str ());
	glUniform1ui (_uniforms[param], value);
}
void Shader::pushUniform (std::string param, GLfloat value) {
	_uniforms[param] = glGetUniformLocation (_programShader, param.c_str ());
	glUniform1f (_uniforms[param], value);
}
void Shader::pushUniform (std::string param, GLint value1, GLint value2) {
	_uniforms[param] = glGetUniformLocation (_programShader, param.c_str ());
	glUniform2i (_uniforms[param], value1, value2);
}
void Shader::pushUniform (std::string param, GLuint value1, GLuint value2) {
	_uniforms[param] = glGetUniformLocation (_programShader, param.c_str ());
	glUniform2ui (_uniforms[param], value1, value2);
}
void Shader::pushUniform (std::string param, GLfloat value1, GLfloat value2) {
	_uniforms[param] = glGetUniformLocation (_programShader, param.c_str ());
	glUniform2f (_uniforms[param], value1, value2);
}
void Shader::pushUniform (std::string param, GLint value1, GLint value2, GLint value3) {
	_uniforms[param] = glGetUniformLocation (_programShader, param.c_str ());
	glUniform3i (_uniforms[param], value1, value2, value3);
}
void Shader::pushUniform (std::string param, GLuint value1, GLuint value2, GLuint value3) {
	_uniforms[param] = glGetUniformLocation (_programShader, param.c_str ());
	glUniform3ui (_uniforms[param], value1, value2, value3);
}
void Shader::pushUniform (std::string param, GLfloat value1, GLfloat value2, GLfloat value3) {
	_uniforms[param] = glGetUniformLocation (_programShader, param.c_str ());
	glUniform3f (_uniforms[param], value1, value2, value3);
}
void Shader::pushUniform (std::string param, GLint value1, GLint value2, GLint value3, GLint value4) {
	_uniforms[param] = glGetUniformLocation (_programShader, param.c_str ());
	glUniform4i (_uniforms[param], value1, value2, value3, value4);
}
void Shader::pushUniform (std::string param, GLuint value1, GLuint value2, GLuint value3, GLuint value4) {
	_uniforms[param] = glGetUniformLocation (_programShader, param.c_str ());
	glUniform4ui (_uniforms[param], value1, value2, value3, value4);
}
void Shader::pushUniform (std::string param, GLfloat value1, GLfloat value2, GLfloat value3, GLfloat value4) {
	_uniforms[param] = glGetUniformLocation (_programShader, param.c_str ());
	glUniform4f (_uniforms[param], value1, value2, value3, value4);
}
void Shader::pushUniform (std::string param, GLsizei count, GLboolean transpose, const GLfloat *value, int size) {
	_uniforms[param] = glGetUniformLocation (_programShader, param.c_str ());
	switch (size) {
	case 2:
		glUniformMatrix2fv (_uniforms[param], count, transpose, value);
		break;
	case 3:
		glUniformMatrix3fv (_uniforms[param], count, transpose, value);
		break;
	case 4:
		glUniformMatrix4fv (_uniforms[param], count, transpose, value);
		break;
	}
}

void Shader::setUniform (std::string param, GLint value) {
	glUniform1i (_uniforms[param], value);
}
void Shader::setUniform (std::string param, GLuint value) {
	glUniform1ui (_uniforms[param], value);
}
void Shader::setUniform (std::string param, GLfloat value) {
	glUniform1f (_uniforms[param], value);
}
void Shader::setUniform (std::string param, GLint value1, GLint value2) {
	glUniform2i (_uniforms[param], value1, value2);
}
void Shader::setUniform (std::string param, GLuint value1, GLuint value2) {
	glUniform2ui (_uniforms[param], value1, value2);
}
void Shader::setUniform (std::string param, GLfloat value1, GLfloat value2) {
	glUniform2f (_uniforms[param], value1, value2);
}
void Shader::setUniform (std::string param, GLint value1, GLint value2, GLint value3) {
	glUniform3i (_uniforms[param], value1, value2, value3);
}
void Shader::setUniform (std::string param, GLuint value1, GLuint value2, GLuint value3) {
	glUniform3ui (_uniforms[param], value1, value2, value3);
}
void Shader::setUniform (std::string param, GLfloat value1, GLfloat value2, GLfloat value3) {
	glUniform3f (_uniforms[param], value1, value2, value3);
}
void Shader::setUniform (std::string param, GLint value1, GLint value2, GLint value3, GLint value4) {
	glUniform4i (_uniforms[param], value1, value2, value3, value4);
}
void Shader::setUniform (std::string param, GLuint value1, GLuint value2, GLuint value3, GLuint value4) {
	glUniform4ui (_uniforms[param], value1, value2, value3, value4);
}
void Shader::setUniform (std::string param, GLfloat value1, GLfloat value2, GLfloat value3, GLfloat value4) {
	glUniform4f (_uniforms[param], value1, value2, value3, value4);
}
void Shader::setUniform (std::string param, GLsizei count, GLboolean transpose, const GLfloat *value, int size) {
	switch (size) {
	case 2:
		glUniformMatrix2fv (_uniforms[param], count, transpose, value);
		break;
	case 3:
		glUniformMatrix3fv (_uniforms[param], count, transpose, value);
		break;
	case 4:
		glUniformMatrix4fv (_uniforms[param], count, transpose, value);
		break;
	}
}

GLuint Shader::getUniform (std::string param) {
	if (_uniforms.count (param))
		return _uniforms[param];
	else
		return -1;
}

}}

