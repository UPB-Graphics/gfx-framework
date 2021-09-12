#version 430

// Input
layout(location = 0) in vec2 texture_coord;

// Uniform properties
uniform sampler2D texture_1;

// Output
layout(location = 0) out vec4 color;


void main()
{
    float depth = pow(texture(texture_1, texture_coord).r, 64);
    color = vec4(depth);
}
