#version 330

// source: https://www.mathematik.uni-marburg.de/~thormae/lectures/graphics1/code/WebGLShaderLightMat/ShaderLightMat.html

uniform vec3 u_light_direction;
uniform vec3 u_view_position;
uniform vec4 u_light_color;
uniform vec4 u_light_ambient;
uniform vec4 u_light_specular;
uniform vec4 u_material_ambient;
uniform vec4 u_material_specular;

float u_material_shininess = 128.0f;

uniform sampler2D u_diffuse;

in vec4 v_diffuse;
in vec2 v_texcoord;
in vec3 v_normal;
in vec3 v_fragment_position;

out vec4 final_color;

void main() {
	vec3 L = -normalize(u_light_direction);
	vec3 N = normalize(v_normal);
	float NdL = max(0.0, dot(N, L));
	vec4 light_color = u_light_color * NdL;

	vec3 R = normalize(reflect(L, N));
	vec3 V = normalize(v_fragment_position - u_view_position);
	float RdV = max(0.0, dot(R, V));

	vec4 ambient = u_material_ambient * u_light_ambient;
	vec4 diffuse = light_color;
	vec4 specular = u_material_specular * pow(RdV, u_material_shininess) * u_light_specular;

	final_color = texture(u_diffuse, v_texcoord) * (ambient + diffuse + specular);
}