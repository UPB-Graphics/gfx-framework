#version 330

// Input
in vec2 texture_coord;
in vec3 world_position;
in vec3 world_normal;

// Uniform properties
uniform sampler2D texture_1;
uniform sampler2D depth_texture;
uniform vec3 eye_position;
uniform vec3 light_position;
uniform vec3 light_direction;
uniform vec3 light_color;

uniform mat4 light_space_view;
uniform mat4 light_space_projection;

uniform float light_space_far_plane;

// Output
layout(location = 0) out vec4 out_color;

// Local variables and functions
const vec3 KA = vec3 (0.25);            // ambient factor
const vec3 KD = vec3 (0.3);             // diffuse factor
const vec3 KS = vec3 (0.3);             // specular factor
const float SPECULAR_EXPONENT = 40.0;   // specular exponent

const float spot_angle = 45.0;

vec3 PhongLight()
{
    vec3 N = normalize(world_normal);
    vec3 L = normalize(light_position - world_position);

    float dist = distance(light_position, world_position);

    if (dist > light_space_far_plane)
        return vec3(0);

    float att = pow((light_space_far_plane - dist) /
        light_space_far_plane, 2);

    float dot_specular = dot(N, L);
    vec3 specular = vec3(0);
    if (dot_specular > 0)
    {
        vec3 V = normalize(eye_position - world_position);
        vec3 H = normalize(L + V);
        specular = KS * pow(max(dot(N, H), 0), SPECULAR_EXPONENT);
    }

    vec3 diffuse = KD * max(dot_specular, 0);

    return att * (diffuse + specular);
}

float ShadowFactor()
{
    vec4 light_space_pos = light_space_projection * light_space_view * vec4 (world_position, 1.0f);

    light_space_pos = light_space_pos / light_space_pos.w;

    float light_space_depth = light_space_pos.z * 0.5f + 0.5f;

    vec2 depth_map_pos = light_space_pos.xy * 0.5f + 0.5f;
    float depth = texture(depth_texture, depth_map_pos).x;

    float bias = 0.0005f;

    return light_space_depth - bias < depth ? 1.0f : 0.0f;
}

vec3 SpotLight()
{
    float light_intensity = 10.0f;

    vec3 L = normalize(light_position - world_position);

    float cut_off = radians(spot_angle);
    float spot_light = dot(-L, light_direction);

    if (spot_light > cos(cut_off)) {

        float spot_light_limit = cos(cut_off);
 
        float linear_att = (spot_light - spot_light_limit) / (1.0f - spot_light_limit);
        float light_att_factor = pow(linear_att, 2);

        // TODO(student): Multiply the shadow factor with the
        // result of the lighting calculation. Don't apply
        // this factor to the ambient component as well.
        return light_att_factor * light_intensity * PhongLight() + KA;
    }

    return KA;
}

void main()
{
    vec4 texture_color = texture(texture_1, texture_coord);

    if (texture_color.a < 0.75)
    {
        discard;
    }

    vec3 color = vec3(texture_color) * SpotLight();

    out_color = vec4(color, 1.0);
}
