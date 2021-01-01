#version 330
 
uniform sampler2D texture_1;
uniform sampler2D texture_2;
 
in vec2 texcoord;

layout(location = 0) out vec4 out_color;

void main()
{
	// TODO : calculate the out_color using the texture2D() function

	out_color = vec4(1);
}