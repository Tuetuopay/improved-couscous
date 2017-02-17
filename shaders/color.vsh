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

#version 330 core

layout(location = 0) in vec3 in_Position;
layout(location = 1) in vec2 in_UV;
layout(location = 2) in vec3 in_Color;
layout(location = 3) in vec3 in_Normal;
layout(location = 4) in mat4 in_Model;

out vec3 ex_Color;
out vec2 ex_UV;
out vec3 ex_Normal;
out vec4 ex_ShadedColor;

uniform mat4 matMVP;

struct Light {
	vec4 ambient, diffuse, specular, position;
	vec3 spotDirection;
	float spotExponent, spotCutoff, spotCosCutoff;
	float attenuationConstant, attenuationLinear, attenuationQuadratic;
};
layout (std140) uniform lights {
	Light in_lights[8];
	bool in_enabled[8];
};

void main() {
	gl_Position = matMVP * in_Model * vec4(in_Position, 1);

	ex_Color = in_Color;
	ex_UV = in_UV;
	ex_Normal = in_Normal;

	ex_ShadedColor = vec4(0.0, 0.0, 0.0, 1.0);

	for (int i = 0; i < 8; i++) {
		if (in_enabled[i]) {
			float intensity = dot(ex_Normal, normalize(in_lights[i].position - gl_Position).xyz);
			ex_ShadedColor.rgb += intensity * in_lights[i].diffuse.rgb;
		}
	}
}

