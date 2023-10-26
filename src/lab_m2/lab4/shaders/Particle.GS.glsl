#version 430

// Input and output topologies
layout(points) in;
layout(triangle_strip, max_vertices = 4) out;

// Uniform properties
uniform mat4 View;
uniform mat4 Projection;
uniform vec3 eye_position;
uniform float offset;

in float vert_lifetime[1];
in float vert_iLifetime[1];

// Output
layout(location = 0) out vec2 texture_coord;
layout(location = 1) out float geom_lifetime;
layout(location = 2) out float geom_iLifetime;


// Local variables
vec3 vpos = gl_in[0].gl_Position.xyz;
vec3 forward = normalize(eye_position - vpos);
vec3 right = normalize(cross(forward, vec3(0, 1, 0)));
vec3 up = normalize(cross(forward, right));


void EmitPoint(vec2 offset)
{
    vec3 pos = right * offset.x + up * offset.y + vpos;
    gl_Position = Projection * View * vec4(pos, 1.0);
    EmitVertex();
}


void main()
{
    float ds = offset;

    // TODO(student): Create a quad based on `triangle_strip`.
    //
    //  3---------2
    //  | \       |
    //  |    \    |
    //  |       \ |
    //  0---------1
    //
    // Triangles: (0, 1, 3), (1, 2, 3)
    // Hint: before you emit a vertex, you have to specify its texture
    // coordinates and pass on the values of vert_lifetime[0] and 
    // vert_ilifetime[0] in geom_lifetime and geom_ilifetime
    // Hint: if a point has the coordinates (0,0), then the quad will have the following coords:
    // (-ds,-ds), (ds,-ds), (ds,ds)....

}
