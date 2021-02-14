#version 410

// Input
layout(location = 0) in vec3 color;
layout(location = 1) in vec3 world_position;
layout(location = 2) in vec3 world_normal;
layout(location = 3) in vec4 shadow_pos;

// Uniform properties
uniform sampler2D texture_depth;

uniform vec3 light_direction;
uniform vec3 light_position;
uniform int light_mode;
uniform int light_power;

uniform vec3 eye_position;
uniform float material_kd;
uniform float material_ks;
uniform int material_shininess;
uniform vec3 object_color;

// Output
layout(location = 0) out vec4 out_color;


void main()
{
    // Fragment position and normals in world space
    vec3 P = world_position;
    vec3 N = normalize(world_normal);

    // ------------------------------------------------------------------------
    // Shadow 

    // NDC to Screen coordinates
    // TODO(student): Check if pixel is in shadow
    float visibility = 1.0;

    // ------------------------------------------------------------------------
    // Phong lighting

    vec3 L = normalize(light_position - P);
    vec3 V = normalize(eye_position - P);

    float ambient_light = 0.25;
    float diffuse_light = 0;
    float specular_light = 0;
    float spot_light_factor = 1;

    float light = ambient_light;

    diffuse_light = max(dot(L, N), 0);
    if (diffuse_light > 0)
    {
        // specular_light = pow(max(dot(V, reflect(-L, N)), 0), material_shininess);    // Phong
        specular_light = pow(max(dot(N, normalize(L + V)), 0), material_shininess);     // Blinn-Phong
    }

    light = ambient_light + material_kd * diffuse_light + material_ks * specular_light;

    vec3 color = object_color * vec3(light);
    out_color = visibility * vec4(color, 1);
}
