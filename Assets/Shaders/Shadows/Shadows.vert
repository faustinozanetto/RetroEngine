#version 460

layout (location = 0) in vec3 aPos;

layout(location = 0) uniform mat4 m_model;

layout (std140, binding = 3) uniform ShadowData
{
    mat4 light_view_projection;
} shadow;

void main(){
    gl_Position = shadow.light_view_projection * m_model * vec4(aPos, 1.0);
}
