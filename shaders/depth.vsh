#version 330 core

layout(location = 0) in vec3 in_Position;
layout(location = 4) in mat4 in_Model;

uniform mat4 matMVP;

void main() {
	gl_Position = matMVP * in_Model * vec4(in_Position, 1);
}




