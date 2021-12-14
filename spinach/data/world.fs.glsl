#version 330

uniform vec3 u_light_direction;
uniform vec3 u_view_position;
uniform vec3 u_light_color;
uniform vec3 u_light_ambient;
uniform vec3 u_light_specular;
uniform vec3 u_material_ambient;
uniform vec3 u_material_specular;

float u_material_shininess = 128.0f;
float visibility = 1.0f;

uniform sampler2D u_diffuse;
uniform sampler2D u_shadowmap;

in vec4 v_diffuse;
in vec2 v_texcoord;
in vec3 v_normal;
in vec3 v_fragment_position;
in vec4 v_position_light_space;

out vec4 final_color;

void main() {
	vec3 projected_coordinates = v_position_light_space.xyz / v_position_light_space.w;
	projected_coordinates = projected_coordinates * 0.5 + 0.5;
	float closest_depth = texture(u_shadowmap, projected_coordinates.xy).r;
	float current_depth = projected_coordinates.z;
	if(closest_depth < current_depth){
		visibility = 0.2f;
	}

	vec3 L = -normalize(u_light_direction);
	vec3 N = normalize(v_normal);
	float NdL = max(0.0, dot(N, L));
	vec3 light_color = u_light_color * NdL;

	vec3 R = normalize(reflect(L, N));
	vec3 V = normalize(v_fragment_position - u_view_position);
	float RdV = max(0.0, dot(R, V));
	vec3 color = texture(u_diffuse, v_texcoord).rgb;
	vec3 ambient  =	u_material_ambient * u_light_ambient;
	vec3 diffuse  = light_color;
	vec3 specular = u_material_specular * pow(RdV, u_material_shininess) * u_light_specular;

	vec3 lighting = color * (ambient + visibility * (diffuse + specular));

	final_color = vec4(lighting, 1.0);
}