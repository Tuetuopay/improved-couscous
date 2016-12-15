#version 330 core

in vec3 ex_Color;
in vec2 ex_UV;
in vec4 ex_Shadow;

out vec4 fragColor;

uniform sampler2D tex;
uniform sampler2D tex_shadow;

void main() {
	float dimming = 1.0;

	//if (texture(tex_shadow, ex_Shadow.xy).x + 0.005 < ex_Shadow.z)
	//	dimming = 0.5;
	// if (greaterThan(ex_Shadow.xy, vec2(0, 0)) == bvec2(true, true) &&
	//     greaterThan(ex_Shadow.xy, vec2(1, 1)) == bvec2(false, false))
	vec2 ex_ShadowProj = ex_Shadow.xy / ex_Shadow.w;
	//if (ex_ShadowProj.x >= 0.0 && ex_ShadowProj.x <= 1.0 &&
	//    ex_ShadowProj.y >= 0.0 && ex_ShadowProj.y <= 1.0) {
	if (length(ex_ShadowProj - vec2(0.5, 0.5)) < 0.5) {
		if (textureProj(tex_shadow, ex_Shadow.xyw).x < (ex_Shadow.z - 0.005) / ex_Shadow.w)
			dimming = 0.5;
	}
	else dimming = 0.5;

	fragColor = texture(tex, ex_UV) * dimming;
}

