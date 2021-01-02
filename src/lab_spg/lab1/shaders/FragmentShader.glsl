#version 430

layout(location = 0) in vec2 texture_coord;

uniform sampler2D texture_1;

layout(location = 0) out vec4 out_color;

void main()
{
	//TODO apply texture

	//TODO discard when alfa component < 0.75

	out_color = vec4(0,0,1,0);
}