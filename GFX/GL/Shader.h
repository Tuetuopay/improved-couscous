//
//  Shader.h
//
//  Copyright (c) 2013 Tuetuopay. All rights reserved.
//

#ifndef _SHADER_H
#define _SHADER_H

#include <iostream>
#include <string>
#include <map>
#include <vector>

#include <GL/glew.h>

namespace GFX { namespace GL {

class Shader
{
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
