#version 430
layout(triangles) in;
layout(triangle_strip, max_vertices = 170) out;

layout(location = 0) in vec2 v_texture_coord[];

uniform mat4 View;
uniform mat4 Projection;

layout(location = 0) out vec2 texture_coord;

void EmitPoint(vec3 pos, vec3 offset)
{
	gl_Position = Projection * View * vec4(pos + offset, 1.0);
	EmitVertex();
}

void main()
{
	vec3 p1 = gl_in[0].gl_Position.xyz;
	vec3 p2 = gl_in[1].gl_Position.xyz;
	vec3 p3 = gl_in[2].gl_Position.xyz;

	texture_coord = v_texture_coord[0];
	EmitPoint(p1, vec3(0));

	texture_coord = v_texture_coord[1];
	EmitPoint(p2, vec3(0));

	texture_coord = v_texture_coord[2];
	EmitPoint(p3, vec3(0));

	EndPrimitive();
}
