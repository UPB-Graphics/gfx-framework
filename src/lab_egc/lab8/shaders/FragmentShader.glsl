#version 330

// Input
// TODO(student): Get color value from vertex shader
in vec3 world_position;
in vec3 world_normal;

// Uniforms for light properties
uniform vec3 light_direction;
uniform vec3 light_position;
uniform vec3 eye_position;

uniform float material_kd;
uniform float material_ks;
uniform int material_shininess;

uniform vec3 object_color;

// Output
layout(location = 0) out vec4 out_color;


void main()
{
    // TODO(student): define ambient light component
    float ambient_light = 0.25;

    // TODO(student): compute diffuse light component
    float diffuse_light = 0;

    // TODO(student): compute specular light component
    float specular_light = 0;

    if (diffuse_light > 0)
    {
    }

    // TODO(student): compute light

    // TODO(student): write pixel out color
    out_color = vec4(1);
}
