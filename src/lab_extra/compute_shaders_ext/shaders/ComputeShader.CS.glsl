#version 430

// Input
layout(local_size_x = 16, local_size_y = 16) in;

// Uniform properties
layout (binding = 0, rgba32f) uniform image2D image;
layout (binding = 1, rgba8) uniform image2D colorBuffer;

// Local variables
shared vec4 samples[32];


void main()
{
    ivec2 pixel = ivec2(gl_GlobalInvocationID.xy);
    vec4 color = vec4(0);
    int k = 10;

    samples[gl_LocalInvocationIndex] = imageLoad(image, pixel);

    barrier();

    for (int i = -k; i <= k; i++)
    {
        for (int j = -k; j <= k; j++)
        {
            color += imageLoad(image, pixel + ivec2(i, j));
        }
    }

    color *= 1.0 / pow(2 * k + 1, 2);
    imageStore(colorBuffer, pixel, color);
}
