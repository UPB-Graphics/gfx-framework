#version 430

// Input
layout(location = 0) in vec2 texture_coord;

// Uniform properties
uniform sampler2D texture_1;

// Output
layout(location = 0) out vec4 out_color;


void main()
{
    vec3 color = texture(texture_1, texture_coord).xyz;
    out_color = vec4(color, 1);
}
