#version 330
in vec3 frag_normal;

uniform sampler2D u_texture_0;

layout(location = 0) out vec4 out_color;

void main()
{
	out_color = vec4(abs(frag_normal), 1);
}