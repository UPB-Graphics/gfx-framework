#version 330

layout(triangles) in;
// TODO(student): Update max_vertices
layout(triangle_strip, max_vertices = 3) out;

uniform mat4 View;
uniform mat4 Projection;
uniform mat4 viewMatrices[6];

in vec3 geom_position[3];
in vec2 geom_texture_coord[3];

out vec3 frag_position;
out vec2 frag_texture_coord;

void main()
{
    int i, layer;

    // TODO(student): Update the code to compute the position from each camera view 
    // in order to render a cubemap in one pass using gl_Layer. Use the "viewMatrices"
    // attribute according to the corresponding layer.

    for (i = 0; i < gl_in.length(); i++) {
         frag_position = geom_position[i];
         frag_texture_coord = geom_texture_coord[i];
         gl_Position = Projection * View * gl_in[i].gl_Position;
         EmitVertex();
    }
    EndPrimitive();
    
}
