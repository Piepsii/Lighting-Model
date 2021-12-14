#version 330

layout (location = 0) in vec4 a_vertex;

out vec2 v_texcoord;

void main() {
	gl_Position = vec4(a_vertex.xy, 0, 1);
	v_texcoord = a_vertex.zw;
}