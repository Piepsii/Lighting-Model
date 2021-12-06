#version 330

layout (location = 0) in vec3 a_position;
layout (location = 1) in vec4 a_diffuse;
layout (location = 2) in vec2 a_texcoord;
layout (location = 3) in vec3 a_normal;

uniform mat4 u_projection;
uniform mat4 u_view;
uniform mat4 u_world;

out vec2 v_texcoord;
out vec3 v_normal;
out vec3 v_position;

void main() {
	gl_Position = u_projection * u_view * u_world * vec4(a_position, 1.0);
	v_position = a_position;
	v_texcoord = a_texcoord;
	v_normal = a_normal;
}