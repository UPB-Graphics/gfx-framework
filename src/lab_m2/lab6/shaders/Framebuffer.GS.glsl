#version 430

layout(triangles) in;
// TODO(student): Update max_vertices
layout(triangle_strip, max_vertices = 3) out;

uniform mat4 View;
uniform mat4 Projection;
uniform mat4 viewMatrices[6];


void main()
{
    int i, layer;

    // TODO(student): Update the code to compute the position from each camera view 
    // in order to render a cubemap in one pass using gl_Layer

    for (i = 0; i < gl_in.length(); i++) {
         gl_Position = Projection * View * gl_in[i].gl_Position;
         EmitVertex();
    }
    EndPrimitive();
    
}
