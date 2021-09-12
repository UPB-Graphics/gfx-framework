#version 430

// Input
layout(location = 0) in vec3 v_position;
layout(location = 1) in vec3 v_normal;
layout(location = 2) in vec2 v_texture_coord;

// Uniform properties
uniform mat4 Projection;
uniform mat4 View;
uniform mat4 Model;


void main()
{
    gl_Position =  Model * vec4(v_position, 1);
}
