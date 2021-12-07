#version 330

layout (location = 0) in vec3 a_position;
layout (location = 1) in vec4 a_diffuse;
layout (location = 2) in vec2 a_texcoord;
layout (location = 3) in vec3 a_normal;

uniform mat4 u_projection;
uniform mat4 u_view;
uniform mat4 u_world;

out vec4 v_diffuse;
out vec2 v_texcoord;
out vec3 v_normal;
out vec3 v_fragment_position;

void main() {
	gl_Position = u_projection * u_view * u_world * vec4(a_position, 1.0);
	v_diffuse = a_diffuse;
	v_texcoord = a_texcoord;
	v_normal = (u_world * vec4(a_normal, 0)).xyz;
	v_fragment_position = vec3(u_world * vec4(a_position, 1.0));
}