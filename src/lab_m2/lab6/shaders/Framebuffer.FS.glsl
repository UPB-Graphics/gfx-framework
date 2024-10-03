#version 330

layout(location = 0) out vec4 out_color;

uniform sampler2D texture_1;
uniform samplerCube texture_cubemap;
uniform int cube_draw;

in vec3 frag_position;
in vec2 frag_texture_coord;

void main()
{
    vec3 color = vec3(0);

    if (cube_draw == 0) {
        color = texture(texture_1, frag_texture_coord).xyz;
    }

    if (cube_draw == 1) {
        color = texture(texture_cubemap, normalize(frag_position)).xyz;
    }

    out_color = vec4(color,1);
}
