#version 430
layout(location = 0) in vec2 text_coord;

layout(location = 0) out vec4 out_color;

void main()
{
	out_color = vec4(text_coord, 0, 0);
}