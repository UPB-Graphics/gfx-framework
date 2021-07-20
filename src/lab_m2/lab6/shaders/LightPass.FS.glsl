#version 430

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
const float SPECULAR_EXPONENT = 40.0;   // specular exponent


vec4 PhongLight(vec3 w_pos, vec3 w_N)
{
    vec3 L = normalize(light_position - w_pos);

    float dist = distance(light_position, w_pos);

    // Ignore fragments outside of the
    // light influence zone (radius)
    if (dist > light_radius)
        return vec4(0);

    float att = pow(light_radius - dist, 2);

    float dot_specular = dot(w_N, L);
    vec3 specular = vec3(0);
    if (dot_specular > 0)
    {
        vec3 V = normalize(eye_position - w_pos);
        vec3 H = normalize(L + V);
        specular = LS * pow(max(dot(w_N, H), 0), SPECULAR_EXPONENT);
    }

    vec3 diffuse = LD * max(dot_specular, 0);

    return vec4(att * (diffuse + specular), 1.0);
}


void main()
{
    vec2 text_coord = gl_FragCoord.xy / resolution;
    vec3 wPos = texture(texture_position, text_coord).xyz;
    vec3 wNorm = texture(texture_normal, text_coord).xyz;
    out_color.xyz = light_color * PhongLight(wPos, wNorm).xyz;
}
