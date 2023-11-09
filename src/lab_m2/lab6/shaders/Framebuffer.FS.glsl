#version 430

layout(location = 0) out vec4 out_color;

uniform sampler2D texture_1;

in vec2 frag_texture_coord;

void main()
{
    vec3 color = texture(texture_1, frag_texture_coord).xyz;

    out_color = vec4(color,1);
}
