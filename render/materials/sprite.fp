varying mediump vec2 var_texcoord0;

uniform lowp sampler2D texture_sampler;
uniform lowp vec4 tint;
uniform lowp vec4 effects; // x - exposure

void main() {
	float exposure = effects.x;
	// Pre-multiply alpha since all runtime textures already are
	lowp vec4 tint_pm = vec4(tint.xyz * tint.w, tint.w);
	vec4 color = texture2D(texture_sampler, var_texcoord0.xy) * tint_pm;
	vec3 mapped = color.rgb * exposure;
	gl_FragColor = vec4(mapped.rgb, color.a);
}
