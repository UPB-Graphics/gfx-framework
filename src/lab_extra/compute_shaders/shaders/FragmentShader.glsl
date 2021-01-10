#version 430

// Input
layout(location = 0) in vec2 texture_coord;
layout(location = 1) in vec4 world_position;
layout(location = 2) in vec3 world_normal;

// Uniform properties
uniform sampler2D texture_1;

// Output
layout(location = 0) out vec4 out_diffuse;
layout(location = 1) out vec4 out_world_pos;
layout(location = 2) out vec4 out_world_normal;


void main()
{
    vec4 color = texture(texture_1, texture_coord);

    if (color.a < 0.75)
    {
        discard;
    }

    out_diffuse = color;
    out_world_pos = world_position;
    out_world_normal.xyz = normalize(world_normal);
}
