#version 330 core

layout(location = 0) in vec2 in_Position;
layout(location = 1) in vec2 in_UV;
layout(location = 4) in vec4 in_Displace;

out vec2 ex_UV;

uniform mat4 matMVP;

void main() {
	gl_Position = matMVP * vec4(in_Position.xy + in_Displace.xy, 0.0, 1.0);

	ex_UV = (in_UV + in_Displace.zw) / 16.0;
}

