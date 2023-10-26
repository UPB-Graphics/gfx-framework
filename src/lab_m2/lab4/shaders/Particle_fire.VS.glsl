#version 430

// Input
layout(location = 0) in vec3 v_position;
layout(location = 1) in vec3 v_normal;
layout(location = 2) in vec2 v_texture_coord;

// Uniform properties
uniform mat4 Model;
uniform vec3 generator_position;
uniform float deltaTime;

out float vert_lifetime;
out float vert_iLifetime;

struct Particle
{
    vec4 position;
    vec4 speed;
    vec4 iposition;
    vec4 ispeed;
    float delay;
    float iDelay;
    float lifetime;
    float iLifetime;
};


layout(std430, binding = 0) buffer particles {
    Particle data[];
};


float rand(vec2 co)
{
    return fract(sin(dot(co.xy, vec2(12.9898, 78.233))) * 43758.5453);
}


void main()
{
    vec3 pos = data[gl_VertexID].position.xyz;
    vec3 spd = data[gl_VertexID].speed.xyz;
    float lifetime = data[gl_VertexID].lifetime;

    // TODO(student): Compute the new lifetime of the fire particle based on the actual lifetime
    // The lifetime should decrease each frame with deltaTime
    
    // TODO(student): Compute the new position as the sum
    // between the actual position and the speed.
    //
    // Because the y coordinate is already set positive since the
    // initialization performed on the cpu => the fire
    // will go downwards.
    //
    // For animations that are independent of the platform,
    // the speed must be multiplied with deltaTime.

    if (lifetime < 0)
    {
        pos = data[gl_VertexID].iposition.xyz;
        lifetime = data[gl_VertexID].iLifetime;
    }

    data[gl_VertexID].position.xyz = pos;
    data[gl_VertexID].lifetime = lifetime;

    vert_lifetime = lifetime;
    vert_iLifetime = data[gl_VertexID].iLifetime;

    gl_Position = Model * vec4(pos + generator_position, 1);
}
