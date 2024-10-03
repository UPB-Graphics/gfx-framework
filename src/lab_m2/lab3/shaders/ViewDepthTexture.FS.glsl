#version 330

// Input
in vec2 texture_coord;

// Uniform properties
uniform sampler2D texture_1;

uniform float light_space_near_plane;
uniform float light_space_far_plane;

// Output
layout(location = 0) out vec4 out_color;

#define zNear light_space_near_plane
#define zFar light_space_far_plane

void main()
{
    float depth = texture(texture_1, texture_coord).x;

    float lDepth = (zNear * zFar / (zFar + depth * (zNear - zFar))) / zFar;

    out_color = vec4 (lDepth, lDepth, lDepth, 0);
}
