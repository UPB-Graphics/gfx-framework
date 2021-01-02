#version 430
layout(location = 0) in vec3 text_coord;

uniform samplerCube texture_cubemap;

layout(location = 0) out vec4 out_color;

void main()
{
    vec3 color = texture(texture_cubemap, text_coord).xyz;
	out_color = vec4(color, 0);
}
