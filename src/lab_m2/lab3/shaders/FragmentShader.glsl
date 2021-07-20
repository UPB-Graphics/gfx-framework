#version 430

// Input
layout(location = 0) in vec2 texture_coord;

// Uniform properties
uniform sampler2D texture_1;

// Output
layout(location = 0) out vec4 out_color;


void main()
{
    vec4 color = texture(texture_1, texture_coord);

    if (color.a < 0.75)
    {
        discard;
    }

    out_color = color;
}
