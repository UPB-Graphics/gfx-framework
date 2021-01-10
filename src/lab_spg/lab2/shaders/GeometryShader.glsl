#version 430

// Input
layout(lines) in;

// Output
// TODO(student): Generate a curve (via line strip),
// then a rotation/translation surface (via triangle strip)
layout(line_strip, max_vertices = 256) out;

// Uniform properties
uniform mat4 View;
uniform mat4 Projection;
uniform vec3 control_p1, control_p2, control_p3, control_p4;
uniform int no_of_instances;

// Input
in int instance[2];


vec3 rotateY(vec3 point, float u)
{
    float x = point.x * cos(u) - point.z *sin(u);
    float z = point.x * sin(u) + point.z *cos(u);
    return vec3(x, point.y, z);
}


vec3 translateX(vec3 point, float t)
{
    return vec3(point.x + t, point.y, point.z);
}


vec3 bezier(float t)
{
    return control_p1 * pow((1 - t), 3) + control_p2 * 3 * t * pow((1 - t), 2) + control_p3 * 3 * pow(t, 2) * (1 - t) + control_p4 * pow(t, 3);
}


// TODO(student): Try to create a Hermite/B-spline curve


void main()
{
    if (instance[0] < no_of_instances)
    {
        // TODO(student): Instead of emitting vertices for the control points,
        // emit vertices that approximate the Bezier curve

        gl_Position = Projection* View * vec4(control_p1, 1);    EmitVertex();
        gl_Position = Projection* View * vec4(control_p2, 1);    EmitVertex();
        gl_Position = Projection* View * vec4(control_p3, 1);    EmitVertex();
        gl_Position = Projection* View * vec4(control_p4, 1);    EmitVertex();
        EndPrimitive();
    }
}
