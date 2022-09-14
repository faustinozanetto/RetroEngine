#version 460

layout(location = 0) in vec3 aPos;
layout(location = 1) in vec4 aColor;

layout(location = 0) out vec3 vPos;
layout(location = 1) out vec4 vColor;

void main() {
    gl_Position = vec4(aPos, 1.0);
    vPos = aPos;
    vColor = aColor;
}