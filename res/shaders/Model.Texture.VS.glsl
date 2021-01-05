#version 330

layout(location = 0) in vec3 v_position;
layout(location = 1) in vec2 v_texture_coord;

uniform mat4 Model;

out vec2 tex_coord;

void main()
{
	tex_coord = v_texture_coord;
	gl_Position = Model * vec4(v_position, 1.0);
}
