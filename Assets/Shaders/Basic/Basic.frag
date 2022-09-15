#version 460

layout(location = 0) in vec3 vPos;
layout(location = 1) in vec2 vTexCoord;
layout(location = 0) out vec4 FragColor;

uniform sampler2D uSampler;

void main() {
    //FragColor = vec4(vPos * 0.5 + 0.5, 1.0);
    FragColor = texture(uSampler, vTexCoord);
}