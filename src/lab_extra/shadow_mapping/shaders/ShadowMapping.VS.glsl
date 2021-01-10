#version 410

// Input
layout(location = 0) in vec3 v_position;
layout(location = 1) in vec3 v_normal;
layout(location = 2) in vec2 v_texture_coord;

// Uniform properties
uniform mat4 Model;
uniform mat4 View;
uniform mat4 Projection;

uniform mat4 LightView;
uniform mat4 LightProjection;

// Output
layout(location = 0) out vec3 color;
layout(location = 1) out vec3 world_position;
layout(location = 2) out vec3 world_normal;
layout(location = 3) out vec4 shadow_pos;


void main()
{
    world_position = (Model * vec4(v_position, 1)).xyz;
    world_normal = normalize(mat3(Model) * v_normal);

    // TODO(student): compute shadow_position using the LightProjection and LightView
    gl_Position = Projection * View * Model * vec4(v_position, 1.0);
}
