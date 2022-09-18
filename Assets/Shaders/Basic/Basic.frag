#version 460

layout(location = 0) in vec3 vPos;
layout(location = 1) in vec2 vTexCoord;
layout(location = 0) out vec4 FragColor;

// Samplers
layout(binding = 0) uniform sampler2D uAlbedoMap;

struct Material {
    vec4 albedo;
    int hasAlbedoMap;
};

uniform Material material;

void main() {
    vec4 albedo = vec4(1.0f);
    if (material.hasAlbedoMap == 1) albedo = texture(uAlbedoMap, vTexCoord);
    else albedo = material.albedo;
    
    FragColor = albedo;
}