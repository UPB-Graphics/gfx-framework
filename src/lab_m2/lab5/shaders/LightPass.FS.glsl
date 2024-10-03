#version 330

// Uniform properties
uniform sampler2D texture_position;
uniform sampler2D texture_normal;

uniform ivec2 resolution;
uniform vec3 eye_position;
uniform vec3 light_position;
uniform vec3 light_color;
uniform float light_radius;

// Output
layout(location = 0) out vec4 out_color;

// Local variables and functions
const vec3 LD = vec3 (0.3);             // diffuse factor
const vec3 LS = vec3 (0.3);             // specular factor
const float SHININESS = 40.0;   // specular exponent

// Computes the output color using Phong lighting model.
// w_pos - world space position of the light source.
// w_N   - world space normal vector of the light source.
vec3 PhongLight(vec3 w_pos, vec3 w_N)
{
    vec3 L = normalize(light_position - w_pos);

    float dist = distance(light_position, w_pos);

    // Ignore fragments outside of the
    // light influence zone (radius)
    if (dist > light_radius)
        return vec3(0);

    float att = pow(light_radius - dist, 2);

    float dot_specular = dot(w_N, L);
    vec3 specular = vec3(0);
    if (dot_specular > 0)
    {
        vec3 V = normalize(eye_position - w_pos);
        vec3 H = normalize(L + V);
        specular = LS * pow(max(dot(w_N, H), 0), SHININESS);
    }

    vec3 diffuse = LD * max(dot_specular, 0);

    return att * (diffuse + specular) * light_color;
}


void main()
{
    vec2 tex_coord = gl_FragCoord.xy / resolution;

    // TODO(student): Sample the texture 'texture_position' at tex_coord to obtain
    // the world space position of the light source.
    vec3 wPos = vec3(0.0);

    // TODO(student): Sample the texture 'texture_normal' at tex_coord to obtain
    // the world space normal vector of the light source.
    vec3 wNorm = vec3(0.0);

    // TODO(student): Compute out_color.rgb using 'PhongLight' method with the world space position 
    // and the world space normal vector of the light source.
    out_color.rgb = vec3(0.2);

    out_color.a = 1.0; 
}
