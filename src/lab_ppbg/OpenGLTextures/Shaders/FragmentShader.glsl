#version 430
 
layout(location = 0) in vec2 texcoord;

uniform sampler2D texture_1;
uniform sampler2D texture_2;
 
layout(location = 0) out vec4 out_color;

void main()
{
	vec4 color = texture2D(texture_1, texcoord);         
	if (color.a < 0.6f)
		discard; 

	out_color = color;
}