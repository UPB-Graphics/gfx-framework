#version 430

// Input
layout(location = 0) in vec2 texture_coord;

// Uniform properties
uniform sampler2D texture_1;
uniform sampler2D texture_2;
uniform sampler2D texture_3;
uniform sampler2D texture_4;
uniform sampler2D texture_5;

uniform int textureID = 0;

// Local variables
vec2 sampleSize = vec2(1.0) / textureSize(texture_1, 0);

// Output
layout(location = 0) out vec4 out_color;


void main()
{
    vec4 color = vec4(0);

    switch (textureID) 
    {
    case 0:
        color = texture(texture_1, texture_coord);
        break;

    case 1:
        color = texture(texture_2, texture_coord);
        //color = fract(color);
        break;

    case 2:
        color = texture(texture_3, texture_coord);
        break;

    case 3:
        color = texture(texture_4, texture_coord);
        break;

    case 4:
        color = texture(texture_5, texture_coord);
        break;

    }

    out_color = color;
}
