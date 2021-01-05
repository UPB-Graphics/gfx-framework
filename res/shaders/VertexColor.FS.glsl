#version 330

in vec3 frag_color;

layout(location = 0) out vec4 out_color;

void main()
{
	out_color = vec4(frag_color, 1);
}