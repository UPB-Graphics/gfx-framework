#version 330

layout(location = 0) in vec3 v_position;

uniform mat4 Model;
uniform mat4 View;
uniform mat4 Projection;

void main()
{
	gl_Position = Projection * View * Model * vec4(v_position, 1.0);
}
