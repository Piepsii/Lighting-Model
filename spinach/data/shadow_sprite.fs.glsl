#version 330

uniform sampler2D u_diffuse;

in  vec2 v_texcoord;

out vec4 final_color;

void main() {
	final_color = vec4(texture(u_diffuse, v_texcoord).rrr, 1.0);
}