#version 430

// Input topology
layout(triangles, equal_spacing, ccw) in;
//layout(triangles, fractional_even_spacing, ccw) in;
//layout(triangles, fractional_odd_spacing, ccw) in;

// Uniform properties
uniform mat4 Projection;
uniform mat4 View;
uniform float time;

// Output
layout(location = 0) out vec4 color;


void main()
{
    vec3 position = gl_TessCoord.x * gl_in[0].gl_Position.xyz + gl_TessCoord.y * gl_in[1].gl_Position.xyz + gl_TessCoord.z * gl_in[2].gl_Position.xyz;

    position.y = (sin(position.x + time) + cos(position.z + time)) / 3;
    color.xyz = abs(position);
    gl_Position = Projection * View * vec4(position, 1);
}
