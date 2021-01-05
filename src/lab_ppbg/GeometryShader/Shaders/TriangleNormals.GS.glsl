#version 430
layout(triangles) in;
layout (line_strip, max_vertices = 6) out;

uniform mat4 View;
uniform mat4 Projection;

const float MAGNITUDE = 0.5;

void EmitPoint(vec3 position)
{
	gl_Position = Projection * View * vec4(position, 1.0);
	EmitVertex();
}

void main()
{
	vec3 p1 = gl_in[0].gl_Position.xyz;
	vec3 p2 = gl_in[1].gl_Position.xyz;
	vec3 p3 = gl_in[2].gl_Position.xyz;
}
