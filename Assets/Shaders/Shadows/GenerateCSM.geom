#version 460

const int NUM_CASCADES = 4;

layout(triangles, invocations = NUM_CASCADES) in;
layout(triangle_strip, max_vertices = 3) out;

uniform mat4 u_light_view_projections[NUM_CASCADES];

void main()
{
    for(int i = 0; i < NUM_CASCADES; ++i)
    {
        gl_Position = u_light_view_projections[gl_InvocationID] * gl_in[i].gl_Position;
        gl_Layer    = gl_InvocationID;
        EmitVertex();
    }
    EndPrimitive();
}