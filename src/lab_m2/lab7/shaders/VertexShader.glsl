#version 430

// Input
layout(location = 0) in vec3 v_position;
layout(location = 1) in vec3 v_normal;
layout(location = 2) in vec2 v_texture_coord;
// TODO (student): add the new attributes neccessary for skinning


const int MAX_BONES = 200;

// Uniform properties
uniform mat4 Model;
uniform mat4 View;
uniform mat4 Projection;
// TODO (student): declare a new uniform variable array, which will
// receive the bone transformations


// Output
layout(location = 0) out vec2 texture_coord;
// TODO (student): send the normal to fragment shader


void main()
{
    // TODO (student): compute the final bone transformation

    texture_coord = v_texture_coord;
    // TODO (student): compute normal

    // TODO (student): apply the bone transformation on the vertex position
    gl_Position = Projection * View * Model * vec4(v_position, 1.0);
}
