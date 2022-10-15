#version 460

struct GeometryOutput {
    vec2 texCoords;
    vec3 position;
    vec3 normal;
    mat3 tbn;
};

struct Material {
    vec4 albedo;
    float metallic;
    float roughness;
    int hasAlbedoMap;
    int hasNormalMap;
    int hasRoughnessMap;
    int hasMetallicMap;
};

uniform Material material;

// Inputs
layout(location = 0) in GeometryOutput geometryInput;

// Samplers
layout(binding = 0) uniform sampler2D uAlbedoMap;
layout(binding = 1) uniform sampler2D uNormalMap;
layout(binding = 2) uniform sampler2D uRoughnessMap;
layout(binding = 3) uniform sampler2D uMetallicMap;

// Outputs
layout(location = 0) out vec3 gPosistion;	
layout(location = 1) out vec4 gAlbedo;
layout(location = 2) out vec3 gNormal;	
layout(location = 3) out vec3 gRoughness;	
layout(location = 4) out vec3 gMetallic;	

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

    /* Roughness */
    if (material.hasRoughnessMap == 1) {
        gRoughness = texture(uRoughnessMap, geometryInput.texCoords).rgb;
    } else {
        gRoughness = vec3(material.roughness);
	}

    /* Metallic */
        if (material.hasMetallicMap == 1) {
        gMetallic = texture(uMetallicMap, geometryInput.texCoords).rgb;
    } else {
        gMetallic = vec3(material.metallic);
    }
}