#version 430

// Input
layout(location = 0) in vec2 texture_coord;

// Uniform properties
uniform sampler2D texture_1;

uniform int textureID = 0;

// Local variables
vec2 sampleSize = vec2(1.0) / textureSize(texture_1, 0);

// Output
layout(location = 0) out vec4 out_color;


void main()
{
    vec4 color = vec4(0);
    int k = 10;
    for (int i = -k; i <= k; i++)
    {
        for (int j = -k; j <= k; j++)
        {
            color += texture(texture_1, texture_coord + ivec2(i, j) * sampleSize);
        }
    }

    color *= 1.0 / pow(2 * k + 1, 2);
    out_color = color;
}
