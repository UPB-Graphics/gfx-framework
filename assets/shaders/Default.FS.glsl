#version 330

// Input
in vec3 frag_normal;
in vec2 tex_coord;

// Uniform properties
uniform sampler2D u_texture_0;

// Output
layout(location = 0) out vec4 out_color;


void main()
{
    out_color = texture(u_texture_0, tex_coord);
    if(out_color.a < 0.9)
    {
        discard;
    }
}
