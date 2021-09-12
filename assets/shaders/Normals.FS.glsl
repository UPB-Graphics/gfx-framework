#version 330

// Input
in vec3 frag_normal;

// Uniform properties
uniform sampler2D u_texture_0;

// Output
layout(location = 0) out vec4 out_color;


void main()
{
    out_color = vec4(abs(frag_normal), 1);
}
