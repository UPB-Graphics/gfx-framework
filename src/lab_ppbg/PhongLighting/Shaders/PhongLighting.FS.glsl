#version 410

// TODO: get color value from vertex shader
layout(location = 0) in vec3 color;
layout(location = 1) in vec3 world_position;
layout(location = 2) in vec3 world_normal;

// TODO: define uniforms for light properties
uniform vec3 light_direction;
uniform vec3 light_position;
uniform int light_mode;
uniform int light_power;

uniform vec3 eye_position;
uniform float material_kd;
uniform float material_ks;
uniform int material_shininess;
uniform vec3 object_color;

layout(location = 0) out vec4 out_color;

void main()
{
	// TODO: compute world space vectors

	// TODO: define ambient light component
	float ambient_light = 0.25;

	// TODO: compute diffuse light component
	float diffuse_light = 0;

	// TODO: compute specular light component
	float specular_light = 0;

	float spot_light_factor = 1;

	float light = ambient_light;

	// Phong lighting in vertex shader
	if (light_mode == 0)				
	{
		out_color = vec4(color, 1);
		return;
	} 
	
	 // Phong lighting in fragment shader
	if (light_mode == 1)
	{
		out_color = vec4(color, 1);
	}

	 // Phong lighting in fragment shader: SpotLight
	if (light_mode == 2)
	{
		out_color = vec4(color, 1);
	}

	light = ambient_light + material_kd * diffuse_light + material_ks * specular_light;

	vec3 color = object_color * vec3(light);
	out_color = vec4(color, 1);
}