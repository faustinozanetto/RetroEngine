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
    float ambient_occlusion;
    int hasAlbedoMap;
    int hasNormalMap;
    int hasRoughnessMap;
    int hasMetallicMap;
    int hasAmbientOcclusionMap;
};

uniform Material material;

// Inputs
layout(location = 0) in GeometryOutput geometryInput;

// Samplers
layout(binding = 0) uniform sampler2D uAlbedoMap;
layout(binding = 1) uniform sampler2D uNormalMap;
layout(binding = 2) uniform sampler2D uRoughnessMap;
layout(binding = 3) uniform sampler2D uMetallicMap;
layout(binding = 4) uniform sampler2D uAmbientOcclusionMap;

// Outputs
layout(location = 0) out vec3 gPosistion;	
layout(location = 1) out vec4 gAlbedo;
layout(location = 2) out vec3 gNormal;	
layout(location = 3) out vec3 gRoughMetalAO;	

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

    if (gAlbedo.a < 0.05)
    discard;

    /* Normal */
    vec3 normal = vec3(1.0);
    if (material.hasNormalMap == 1) {
        normal = texture(uNormalMap, geometryInput.texCoords).rgb;
        normal = normalize(normal * 2.0 - 1.0);
    }
    gNormal = normalize(geometryInput.tbn * normal);

    /* Roughness */
    float roughness;
    if (material.hasRoughnessMap == 1) {
        roughness = texture(uRoughnessMap, geometryInput.texCoords).g;
    } else {
        roughness = material.roughness;
	}

    /* Metallic */
    float metallic;
    if (material.hasMetallicMap == 1) {
        metallic = texture(uMetallicMap, geometryInput.texCoords).b;
    } else {
        metallic = material.metallic;
    }

    /* AO */
    float ao;
    if (material.hasAmbientOcclusionMap == 1) {
        ao = texture(uAmbientOcclusionMap, geometryInput.texCoords).r;
    } else {
        ao = material.ambient_occlusion;
    }

    gRoughMetalAO = vec3(roughness, metallic, ao);
}