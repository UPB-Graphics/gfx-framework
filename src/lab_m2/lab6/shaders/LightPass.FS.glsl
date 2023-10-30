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
const float SHININESS = 40.0;   // specular exponent


vec3 PhongLight(vec3 w_pos, vec3 w_N)
{
    vec3 L = normalize(light_position - w_pos);

    float dist = distance(light_position, w_pos);

    // Ignore fragments outside of the
    // light influence zone (radius)
    if (dist > light_radius)
        return vec3(0);

    float att = pow(light_radius - dist, 2);

    //TODO(student): Compute using Phong lighting the color to be added to the light accumulation.
    // Use the following values:
    //      att - light attenuation
    //      eye_position - position of camera in world space
    //      w_pos - position of fragment in world space
    //      w_N - normal of fragment in world space
    //      L - vector from w_pos to light_position
    //      LD - constant specular factor (defined above)
    //      LS - constant specular factor (defined above)
    //      SHININESS - material shininess (defined above)
    //      light_color - the color of the light source

    return vec3(0.3 * light_color);
}


void main()
{
    vec2 tex_coord = gl_FragCoord.xy / resolution;
    vec3 wPos = texture(texture_position, tex_coord).xyz;
    vec3 wNorm = texture(texture_normal, tex_coord).xyz;
    out_color.rgb = PhongLight(wPos, wNorm);
    out_color.a = 1.0; 
}
