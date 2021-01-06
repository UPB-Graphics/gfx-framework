#version 430

layout(vertices = 3) out;
uniform int tess_factor_inner = 1;
uniform int tess_factor_outer = 1;

void main()
{
	// for triangles only the first 3 outer levels are used
	gl_TessLevelOuter[0] = tess_factor_outer;
	gl_TessLevelOuter[1] = tess_factor_outer;
	gl_TessLevelOuter[2] = tess_factor_outer;
	
	// for triangles only the first inner level is used
	gl_TessLevelInner[0] = tess_factor_inner;

	// computed once per patch
	gl_out[gl_InvocationID].gl_Position = gl_in[gl_InvocationID].gl_Position;
}
