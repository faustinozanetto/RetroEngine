#version 460

struct GeometryOutput {
    vec2 texCoords;
    vec3 position;
    vec3 normal;
    mat3 tbn;
};

struct Material {
    vec4 albedo;
    int hasAlbedoMap;
    int hasNormalMap;
};

uniform Material material;

// Inputs
layout(location = 0) in GeometryOutput geometryInput;

// Samplers
layout(binding = 0) uniform sampler2D uAlbedoMap;
layout(binding = 1) uniform sampler2D uNormalMap;

// Outputs
layout(location = 0) out vec3 gPosistion;	
layout(location = 1) out vec4 gAlbedo;
layout(location = 2) out vec3 gNormal;	

void main() {
    gPosistion = geometryInput.position;

    /* Albedo */
    vec4 albedo = vec4(1.0);
    if (material.hasAlbedoMap == 1) {
        albedo = texture(uAlbedoMap, geometryInput.texCoords);
    } else {
        albedo = material.albedo;
    }
    gAlbedo = albedo;

    /* Normal */
    vec3 normal = vec3(1.0);
    if (material.hasNormalMap == 1) {
        normal = texture(uNormalMap, geometryInput.texCoords).rgb;
        normal = normalize(normal * 2.0 - 1.0);
    }
    gNormal = normalize(geometryInput.tbn * normal);
}