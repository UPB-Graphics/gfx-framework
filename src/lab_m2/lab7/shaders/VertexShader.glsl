#version 330

// Input
layout(location = 0) in vec3 v_position;
layout(location = 1) in vec3 v_normal;
layout(location = 2) in vec2 v_texture_coord;
// TODO (student): Add the new attributes neccessary for skinning


const int MAX_BONES = 200;

// Uniform properties
uniform mat4 Model;
uniform mat4 View;
uniform mat4 Projection;
// TODO (student): Declare a new uniform variable array, which will
// receive the bone transformations


// Output
out vec2 texture_coord;
// TODO (student): Send the normal to the fragment shader


void main()
{
    // TODO (student): Compute the final bone transformation

    texture_coord = v_texture_coord;
    // TODO (student): Compute the normal

    // TODO (student): Apply the bone transformation on the vertex position
    gl_Position = Projection * View * Model * vec4(v_position, 1.0);
}
