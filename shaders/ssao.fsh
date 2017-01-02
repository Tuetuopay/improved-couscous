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

in vec2 ex_UV;

out vec4 fragColor;

// FBO
uniform sampler2D tex_color;
uniform sampler2D tex_depth;
uniform sampler2D tex_shadow;
uniform sampler2D tex_shadow_proj;

// Static parameters
uniform vec2 camerarange;
uniform vec2 screensize;

#define AO_CAP          1.0
#define AO_MULTIPLIER   10000.0
#define DEPTH_TOLERANCE 0.000
// units in space the AO effect extends to (this gets divided by the camera far range)
#define AO_RANGE        10.0

float readDepth(in vec2 coord) {
	return (2.0 * camerarange.x) /
            (camerarange.y + camerarange.x - texture(tex_depth, coord).x *
                                             (camerarange.y - camerarange.x));
}

float compareDepths(in float depth1, in float depth2) {
	float diff = sqrt (clamp (1.0 - (depth1-depth2) / (AO_RANGE/ (camerarange.y-camerarange.x)),
							   0.0,1.0) );
	return min (AO_CAP,
				max (0.0, depth1 - depth2 - DEPTH_TOLERANCE) * AO_MULTIPLIER) * diff;
}

void main() {
	float depth = readDepth(ex_UV);
	float d;

	float pw = 1.0 / screensize.x;
	float ph = 1.0 / screensize.y;

	float ao = 0.1;
	float aoMultiplier = AO_MULTIPLIER;

	float aoscale = 1.0;

	int passes = 4, i = 0;

	for (i = 0; i < passes; i++) {
		d = readDepth(vec2(ex_UV.x + pw, ex_UV.y + ph));
		ao += compareDepths(depth, d) / aoscale;

		d = readDepth( vec2(ex_UV.x - pw, ex_UV.y + ph));
		ao += compareDepths(depth, d) / aoscale;

		d = readDepth( vec2(ex_UV.x + pw, ex_UV.y - ph));
		ao += compareDepths(depth, d) / aoscale;

		d = readDepth( vec2(ex_UV.x - pw, ex_UV.y - ph));
		ao += compareDepths(depth, d) / aoscale;

		pw *= 2.0;
		ph *= 2.0;
		aoMultiplier /= 2.0;
		aoscale *= 1.2;
	}

	ao /= 16.0;
	ao = clamp((1.0 - ao + 0.2), 0.0, 1.0);

	fragColor.a = texture(tex_color, ex_UV.st).a;

#if 1==2
	if (ex_UV.s <= 1.0/6.0 && ex_UV.t <= 1.0/6.0) {
		fragColor.rgb = vec3(ao); // texture (tex_depth, ex_UV.st * 6.0).rgb;
	}
	else {
		fragColor.rgb = ao * texture(tex_color, ex_UV.st).rgb * 1.2;
	}
#else
	float alpha = 0.0;
	ivec2 xy = ivec2(-3, -3);
	for (xy.x = -3; xy.x <= 3; xy.x++)
		for (xy.y = -3; xy.y <= 3; xy.y++)
			alpha += texture(tex_shadow_proj, ex_UV.st + vec2(2.0 / 1280.0, 2.0 / 720.0) * xy).a;
	alpha /= 49.0;
	fragColor.rgb = ao * alpha * texture(tex_color, ex_UV.st).rgb * 1.2;

	if (ex_UV.s <= 1.0/6.0 && ex_UV.t <= 1.0/6.0) {
		// fragColor.rgb = vec3(1, 1, 1) * texture(tex_shadow, ex_UV.st * 6.0).r;
		fragColor.rgb = vec3((ao > 0.9 || ao < 0.1) ? 1.0 : 0.0);
	}
#endif
}

