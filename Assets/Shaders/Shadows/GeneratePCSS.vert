﻿#version 460 core

layout (location = 0) in vec3 aPos;

layout(location = 0) uniform mat4 u_model;

layout(std140, binding = 4) uniform Shadow
{
    mat4 light_view_projection;
    mat4 light_view;
} shadow;

void main()
{
    gl_Position = shadow.light_view_projection * u_model * vec4(aPos, 1.0);
}