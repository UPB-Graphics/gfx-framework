#version 430

// Input
layout(local_size_x = 16, local_size_y = 16) in;

// Uniform properties
layout (binding = 0, rgba32f) uniform image2D image;
layout (binding = 1, rgba8) uniform image2D colorBuffer;


void main()
{
    ivec2 pixel = ivec2(gl_GlobalInvocationID.xy);
    vec4 color = imageLoad(image, pixel);
    imageStore(colorBuffer, pixel, color);
}
