#version 330

layout(location = 0) out float a_fragment_depth;

void main() {
	a_fragment_depth = gl_FragCoord.z;
}