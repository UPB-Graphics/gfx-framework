#version 430

// Output topology
layout(vertices = 3) out;

// Uniform properties
uniform int tess_factor_inner = 1;
uniform int tess_factor_outer = 1;


void main()
{
    // For triangles only the first 3 outer levels are used
    gl_TessLevelOuter[0] = tess_factor_outer;
    gl_TessLevelOuter[1] = tess_factor_outer;
    gl_TessLevelOuter[2] = tess_factor_outer;
    
    // For triangles only the first inner level is used
    gl_TessLevelInner[0] = tess_factor_inner;

    // Computed once per patch
    gl_out[gl_InvocationID].gl_Position = gl_in[gl_InvocationID].gl_Position;
}
