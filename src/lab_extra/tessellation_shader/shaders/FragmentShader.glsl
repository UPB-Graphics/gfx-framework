#version 430

// Input
layout(location = 0) in vec4 color;

// Output
layout(location = 0) out vec4 out_color;


void main()
{
    out_color = color;
}
