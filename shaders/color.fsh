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

in vec4 ex_Position;
in vec3 ex_Color;
in vec2 ex_UV;
in vec3 ex_Normal;

out vec4 fragColor;

uniform sampler2D tex;
uniform vec3 camera;

struct Light {
	vec4 ambient, diffuse, specular, position;
	vec3 spotDirection;
	float spotExponent, spotCutoff, spotCosCutoff;
	float attenuationConstant, attenuationLinear, attenuationQuadratic;
	int enabled;
};
layout (std140) uniform lights {
	Light in_lights[8];
};

vec3 computeLight(Light light) {
	if (light.enabled == 1) {
		vec3 lightDir = (light.position - ex_Position).xyz,
		     viewDir = normalize(camera - ex_Position.xyz);
		float d = length(lightDir);
		vec3 H = normalize(viewDir + lightDir);

		return (dot(ex_Normal, normalize(lightDir))
		        + pow(max(0.0, dot(ex_Normal, H)), 20.0))
		       * (1.0 / (light.attenuationConstant
		                 + d * light.attenuationLinear
		                 + d * d * light.attenuationQuadratic))
		       * light.diffuse.rgb;
	}
	return vec3(0);
}


void main() {
	fragColor = vec4(0.0, 0.0, 0.0, 1.0);
	fragColor.rgb += computeLight(in_lights[0]);
	fragColor.rgb += computeLight(in_lights[1]);
	fragColor.rgb += computeLight(in_lights[2]);
	fragColor.rgb += computeLight(in_lights[3]);
	fragColor.rgb += computeLight(in_lights[4]);
	fragColor.rgb += computeLight(in_lights[5]);
	fragColor.rgb += computeLight(in_lights[6]);
	fragColor.rgb += computeLight(in_lights[7]);
}

