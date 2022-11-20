#version 460

// Tex coords from vertex shader.
layout(location = 0) in vec2 TexCoords;

struct PointLight {
    vec3 position;
    vec3 color;
    float intensity;
    float radius;
};

struct DirectionalLight {
    vec3 direction;
    vec3 color;
    float intensity;
};

// Camera UB
layout(std140, binding = 0) uniform Camera
{
    mat4 u_ViewProjectionMatrix;
    mat4 u_ViewMatrix;
    mat4 u_ProjectionMatrix;
    vec3 u_Position;
} camera;

// Lights UB
layout(std140, binding = 1) uniform Lights
{
    PointLight pointLight;
    DirectionalLight directionalLight;
} lights;

//GBuffer samplers
layout(binding = 0) uniform sampler2D gPosition;
layout(binding = 1) uniform sampler2D gAlbedo;
layout(binding = 2) uniform sampler2D gNormal;
layout(binding = 3) uniform sampler2D gRoughMetalAO;

layout(binding = 4) uniform samplerCube gIrradiance;
layout(binding = 5) uniform samplerCube gPrefilter;
layout(binding = 6) uniform sampler2D gBRDFLut;
layout(binding = 7) uniform sampler2D gShadowMap;

layout(location = 0) out vec4 FragColor;

const float PI = 3.14159265359;
// ----------------------------------------------------------------------------
float DistributionGGX(vec3 N, vec3 H, float roughness)
{
    float a = roughness * roughness;
    float a2 = a * a;
    float NdotH = max(dot(N, H), 0.0);
    float NdotH2 = NdotH * NdotH;

    float num = a2;
    float denom = (NdotH2 * (a2 - 1.0) + 1.0);
    denom = PI * denom * denom;

    return num / max(denom, 1e-5);
}
// ----------------------------------------------------------------------------
float GeometrySchlickGGX(float NdotV, float roughness)
{
    float r = (roughness + 1.0);
    float k = (r * r) / 8.0;

    float num   = NdotV;
    float denom = NdotV * (1.0 - k) + k;

    return num / max(denom, 1e-5);
}
// ----------------------------------------------------------------------------
float GeometrySmith(vec3 N, vec3 V, vec3 L, float roughness)
{
    float NdotV = max(dot(N, V), 0.0);
    float NdotL = max(dot(N, L), 0.0);

    float ggx1 = GeometrySchlickGGX(NdotL, roughness);
    float ggx2 = GeometrySchlickGGX(NdotV, roughness);

    return ggx1 * ggx2;
}
// ----------------------------------------------------------------------------
vec3 FresnelSchlick(float cosTheta, vec3 F0)
{
    return F0 + (1.0 - F0) * pow(max(1.0 - cosTheta, 0.0), 5.0);
}
// ----------------------------------------------------------------------------
vec3 FresnelSchlickRoughness(float cosTheta, vec3 F0, float roughness)
{
    return F0 + (max(vec3(1.0 - roughness), F0) - F0) * pow(max(1.0 - cosTheta, 0.0), 5.0);
}

vec3 unchartedToneMap(vec3 colour)
{
    float A = 0.15;
    float B = 0.50;
    float C = 0.10;
    float D = 0.20;
    float E = 0.02;
    float F = 0.30;
    float W = 11.2;
    float exposure = 2.;
    colour *= exposure;
    colour = ((colour * (A * colour + C * B) + D * E) / (colour * (A * colour + B) + D * F)) - E / F;
    float white = ((W * (A * W + C * B) + D * E) / (W * (A * W + B) + D * F)) - E / F;
    colour /= white;

    return colour;
}

vec3 ACESToneMap(vec3 color)
{
    mat3 m1 = mat3(
    0.59719, 0.07600, 0.02840,
    0.35458, 0.90834, 0.13383,
    0.04823, 0.01566, 0.83777
    );
    mat3 m2 = mat3(
    1.60475, -0.10208, -0.00327,
    -0.53108, 1.10813, -0.07276,
    -0.07367, -0.00605, 1.07602
    );
    vec3 v = m1 * color;
    vec3 a = v * (v + 0.0245786) - 0.000090537;
    vec3 b = v * (0.983729 * v + 0.4329510) + 0.238081;
    return clamp(m2 * (a / b), 0.0, 1.0);
}

vec3 CalculatePointLightPBR(PointLight light, vec3 FragPos, vec3 CamPos, vec3 Albedo, vec3 Normal, float Metallic, float Roughness) {
    //---------------------------------------> View direction (V)
    vec3 V = normalize(CamPos - FragPos);
    vec3 L = normalize(light.position.rgb - FragPos);
    vec3 H = normalize(V + L);
    //----------------------------------------> Fresnel
    vec3 F0 = vec3(0.04);
    F0 = mix(F0, Albedo, Metallic);
    //----------------------------------------> Fresnel
    vec3 F = FresnelSchlickRoughness(max(dot(Normal, V), 0.0), F0, Roughness);
    //----------------------------------------> Cook-Torrance BRDF
    float NDF = DistributionGGX(Normal, H, Roughness);
    float G   = GeometrySmith(Normal, V, L, Roughness);

    float NdotL    = max(dot(Normal, L), 0.0);
    vec3  num      = NDF * G * F;
    float denom    = 4.0 * max(dot(Normal, V), 0.0) * NdotL;
    vec3  specular = num / max(denom, 1e-5);

    vec3 kS = F;
    vec3 kD = 1.0 - kS;
    kD *= 1.0 - Metallic;

    vec3 Radiance = light.color.rgb * 1.0;

    return (kD * Albedo / PI + specular) * Radiance * NdotL;
}

vec3 CalculateIndirectLighting(vec3 FragPos, vec3 CamPos, vec3 Albedo, vec3 Normal, float Metallic, float Roughness, float AO) {
    //---------------------------------------> View direction (V)
    vec3 V = normalize(CamPos - FragPos);
    vec3 R = reflect(-V, Normal);
    //----------------------------------------> Specular reflectance at normal incidence
    vec3 F0 = vec3(0.04);
    F0 = mix(F0, Albedo, Metallic);
    //----------------------------------------> Fresnel
    vec3 F = FresnelSchlickRoughness(max(dot(Normal, V), 0.0), F0, Roughness);
    vec3 kS = F;
    vec3 kD = 1.0 - kS;
    kD *= 1.0 - Metallic;
    //----------------------------------------> Diffuse IBL
    vec3 irradiance = texture(gIrradiance, Normal).rgb;
    vec3 diffuse      = irradiance * Albedo;
    //----------------------------------------> Specular IBL
    const float MAX_REFLECTION_LOD = 4.0;
    vec3 prefilteredColor = textureLod(gPrefilter, R, Roughness * MAX_REFLECTION_LOD).rgb;
    vec2 BRDF  = texture(gBRDFLut, vec2(max(dot(Normal, V), 0.0), Roughness)).rg;
    vec3 specular = prefilteredColor * (F * BRDF.x + BRDF.y);

    return (kD * diffuse * specular) * AO;
}

void main() {
    vec3 FragPos = texture(gPosition, TexCoords).rgb;
    vec4 AlbedoSrc = texture(gAlbedo, TexCoords);
    vec3 Albedo = AlbedoSrc.rgb;
    float Alpha = AlbedoSrc.a;
    vec3 N = texture(gNormal, TexCoords).rgb;
    float Roughness = texture(gRoughMetalAO, TexCoords).r;
    float Metallic  = texture(gRoughMetalAO, TexCoords).g;
    float AO        = texture(gRoughMetalAO, TexCoords).b;
    vec3 CamPos = camera.u_Position;

    vec3 Lighting = vec3(0);
    for (uint i = 0; i < 1; i++){
        Lighting += CalculatePointLightPBR(lights.pointLight, FragPos, CamPos, Albedo, N, Metallic, Roughness);
    }
    // Indirect IBL Lighting
    vec3 IndirectLighthing = CalculateIndirectLighting(FragPos, CamPos, Albedo, N, Metallic, Roughness, AO);
    vec3 color = IndirectLighthing + Lighting;

    // HDR tonemapping
    color = color / (color + vec3(1.0));
    // gamma correct
    color = pow(color, vec3(1.0/2.2));

    FragColor = vec4(color, Alpha);
}