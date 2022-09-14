#version 460

layout(location = 0) in vec3 vPos;
layout(location = 1) in vec4 vColor;
layout(location = 0) out vec4 FragColor;

void main() {
    FragColor = vec4(vPos * 0.5 + 0.5, 1.0);
}