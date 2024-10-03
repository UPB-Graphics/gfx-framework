#version 330

// Input
in vec3 text_coord;

// Uniform properties
uniform samplerCube texture_cubemap;

// Output
layout(location = 0) out vec4 out_color;


void main()
{
    vec3 color = texture(texture_cubemap, text_coord).xyz;
    out_color = vec4(color, 0);
}
