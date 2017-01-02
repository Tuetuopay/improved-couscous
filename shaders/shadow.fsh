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

in vec3 ex_Color;
in vec2 ex_UV;
in vec4 ex_Shadow;

out vec4 fragColor;

uniform sampler2D tex;
uniform sampler2DShadow tex_shadow;

void main() {
	float dimming = 1.0;

	//if (texture(tex_shadow, ex_Shadow.xy).x + 0.005 < ex_Shadow.z)
	//	dimming = 0.5;
	// if (greaterThan(ex_Shadow.xy, vec2(0, 0)) == bvec2(true, true) &&
	//     greaterThan(ex_Shadow.xy, vec2(1, 1)) == bvec2(false, false))
	vec2 ex_ShadowProj = ex_Shadow.xy / ex_Shadow.w;
	//if (ex_ShadowProj.x >= 0.0 && ex_ShadowProj.x <= 1.0 &&
	//    ex_ShadowProj.y >= 0.0 && ex_ShadowProj.y <= 1.0) {
	if (length(ex_ShadowProj - vec2(0.5, 0.5)) < 0.5)
		dimming = mix(0.5, 1.0, textureProj(tex_shadow, ex_Shadow - vec4(0.0, 0.0, 0.005, 0.0)));
	else dimming = 0.5;

	fragColor = vec4(dimming);
}

