#version 330

// Input
in vec2 text_coord;
in vec3 world_position;
in vec3 world_normal;

// Uniform properties
uniform sampler2D u_texture_0;

// Output
layout(location = 0) out vec4 out_world_position;
layout(location = 1) out vec4 out_world_normal;
layout(location = 2) out vec4 out_color;


void main()
{
    out_world_position = vec4(world_position, 1);
    out_world_normal = vec4(normalize(world_normal), 0);
    out_color = texture(u_texture_0, text_coord);
}
