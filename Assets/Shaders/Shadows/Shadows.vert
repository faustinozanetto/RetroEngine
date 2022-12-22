#version 460

const int NUM_CASCADES = 4;

layout (location = 0) in vec3 aPos;
layout (location = 1) in vec2 aTexCoord;
layout (location = 2) in vec3 aNormal;

uniform mat4 m_model;
uniform mat4 u_light_view_projections[NUM_CASCADES];
uniform mat4 u_light_views[NUM_CASCADES];

layout (location = 0) out vec2 out_texcoord;
layout (location = 1) out vec3 out_world_pos;
layout (location = 2) out vec3 out_normal;
layout (location = 3) out vec3 out_view_pos;
layout (location = 4) out vec4 out_pos_light_view_space[NUM_CASCADES];
layout (location = 11) out vec4 out_pos_light_clip_space[NUM_CASCADES];

// Camera UB
layout(std140, binding = 0) uniform Camera
{
    mat4 u_ViewProjectionMatrix;
    mat4 u_ViewMatrix;
    mat4 u_ProjectionMatrix;
    vec3 u_Position;
} camera;

void main()
{
    out_world_pos = vec3(m_model * vec4(aPos, 1.0));
    out_texcoord  = aTexCoord;
    out_normal    = mat3(transpose(inverse(m_model))) * aNormal;

    for (int i = 0; i < NUM_CASCADES; ++i)
    {
        out_pos_light_view_space[i] = u_light_views[i] * vec4(out_world_pos, 1.0);
        out_pos_light_clip_space[i] = u_light_view_projections[i] * vec4(out_world_pos, 1.0);
    }

    gl_Position = camera.u_ViewProjectionMatrix * m_model * vec4(aPos, 1.0);
    out_view_pos = vec3(camera.u_ViewMatrix * m_model * vec4(aPos, 1.0));
}
