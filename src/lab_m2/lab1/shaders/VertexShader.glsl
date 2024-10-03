#version 330

// Input
layout(location = 0) in vec3 v_position;
layout(location = 1) in vec3 v_normal;
layout(location = 2) in vec2 v_texture_coord;

// Uniform properties
uniform mat4 Model;

// Output
out vec2 g_texture_coord;


void main()
{
    g_texture_coord = v_texture_coord;

    gl_Position = Model * vec4(v_position, 1);
}
