#version 410
layout(location = 0) in vec3 normal;
layout(location = 1) in vec2 texture_coord;

uniform sampler2D u_texture_0;
uniform vec3 color;

layout(location = 0) out vec4 frag_color;

void main()
{
	frag_color = vec4(abs(color), 1);
}