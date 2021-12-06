#version 330

layout (location = 0) in vec3 a_position;

uniform mat4 u_light_space_matrix;
uniform mat4 u_model;

void main() {
	gl_Position = light_space_matrix * model * vec4(a_position, 1.0);
}