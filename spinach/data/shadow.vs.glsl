#version 330

layout (location = 0) in vec3 a_position;

uniform mat4 u_depth_MVP;

void main() {
	gl_Position = u_depth_MVP * vec4(a_position, 1.0);
}
