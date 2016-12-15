#version 330 core

layout(location = 0) in vec3 in_Position;
layout(location = 1) in vec2 in_UV;
layout(location = 2) in vec3 in_Color;
layout(location = 4) in mat4 in_Model;

out vec3 ex_Color;
out vec2 ex_UV;
out vec4 ex_Shadow;

uniform mat4 matMVP;
uniform mat4 lightMVP;

void main() {
	vec4 pos = in_Model * vec4(in_Position, 1);
	gl_Position = matMVP * pos;
	ex_Shadow = lightMVP * pos;

	ex_Color = in_Color;
	ex_UV = in_UV;
}

