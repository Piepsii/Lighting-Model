#version 330

// source: https://www.mathematik.uni-marburg.de/~thormae/lectures/graphics1/code/WebGLShaderLightMat/ShaderLightMat.html

uniform vec3 u_light_direction;
uniform vec4 u_light_color;

uniform sampler2D u_diffuse;

in vec4 v_diffuse;
in vec2 v_texcoord;
in vec3 v_normal;

out vec4 final_color;

void main() {
	vec3 L = -normalize(u_light_direction);
	vec3 N = normalize(v_normal);
	float NdL = max(0.0, dot(N, L));
	vec4 light_color = u_light_color * NdL;
	final_color = texture(u_diffuse, v_texcoord) * light_color;
}