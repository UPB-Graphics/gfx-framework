#version 330

// Uniform properties
uniform vec4 debug_color;

// Output
layout(location = 0) out vec4 out_color;


void main()
{
    out_color = debug_color;
}
