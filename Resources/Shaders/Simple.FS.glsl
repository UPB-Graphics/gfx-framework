#version 330

uniform vec4 debug_color;

layout(location = 0) out vec4 out_color;

void main()
{
	out_color = debug_color;
}