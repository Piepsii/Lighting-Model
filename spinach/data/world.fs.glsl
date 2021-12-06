#version 330

// source: https://www.mathematik.uni-marburg.de/~thormae/lectures/graphics1/code/WebGLShaderLightMat/ShaderLightMat.html

in vec3 v_position;
in vec4 v_diffuse;
in vec2 v_texcoord;
in vec3 v_normal;

uniform sampler2D u_diffuse;
uniform vec3 u_light_direction;

const vec4 ambient_color = vec4(0.094, 0.0, 0.0, 1.0);
const vec4 specular_color = vec4(1.0, 1.0, 1.0, 1.0);
const float shininess = 20.0;
const vec4 light_color = vec4(1.0, 1.0, 1.0, 1.0);

vec3 phong_brdf(vec3 light_direction,
				vec3 view_direction, vec3 normal,
				vec3 phong_diffuse_color,
				vec3 phong_specular_color,
				float phong_shininess){
				vec3 color = phong_diffuse_color;
				vec3 reflect_direction = reflect(-light_direction, normal);
				float specular_dot = max(dot(reflect_direction, view_direction), 0.0);
				color += pow(specular_dot, phong_shininess) * phong_specular_color;
				return color;
}

out vec4 final_color;

void main() {
	// vec3 light_direction = normalize(-light_direction);
	// vec3 view_direction = normalize(-v_position);
	// vec3 n = normalize(v_normal);
	// 
	// vec3 luminance = ambient_color.rgb;
	// 
	// float illuminance = dot(light_direction, n);
	// if(illuminance > 0.0){
	// 	vec3 brdf = phong_brdf(light_direction,
	// 						   view_direction,
	// 						   n,
	// 						   v_diffuse.rgb,
	// 						   specular_color.rgb,
	// 						   shininess);
	// luminance += brdf * illuminance * light_color.rgb;
	// }
	// 
	// final_color.rgb = luminance;
	// final_color.a = 1.0;

	final_color = texture(u_diffuse, v_texcoord);
}