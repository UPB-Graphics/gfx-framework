#version 430
layout(triangles) in;
layout(triangle_strip, max_vertices = 170) out;

layout(location = 0) in vec2 v_texture_coord[];

uniform mat4 View;
uniform mat4 Projection;

uniform int instances;
uniform float shrink;

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

	vec3 center = (p1 + p2 + p3) / 3.0f;

	int cols = 3;

	vec3 newp1 = p1 + (center - p1) * shrink;
	vec3 newp2 = p2 + (center - p2) * shrink;
	vec3 newp3 = p3 + (center - p3) * shrink;

	vec3 instanceOffset = vec3(4, 0, 3);

	for (int i = 0; i <= instances; i++)
	{
		int row = i / cols;
		int col = i % cols;
		vec3 offset = instanceOffset * vec3(col, 0, -row);

		texture_coord = v_texture_coord[0];
		EmitPoint(newp1, offset);

		texture_coord = v_texture_coord[1];
		EmitPoint(newp2, offset);

		texture_coord = v_texture_coord[2];
		EmitPoint(newp3, offset);

		EndPrimitive();
	}
}
