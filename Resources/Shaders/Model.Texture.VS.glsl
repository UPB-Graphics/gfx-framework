#version 410

layout(location = 0) in vec3 v_position;
layout(location = 1) in vec2 v_texture_coord;

uniform mat4 Model;

layout(location = 0) out vec2 texture_coord;

void main() {
	texture_coord = v_texture_coord;
	gl_Position = Model * vec4(v_position, 1.0);
}
