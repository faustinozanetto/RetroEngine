#version 460

const int NUM_CASCADES = 4;

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

uniform float u_cascade_splits[NUM_CASCADES];

vec3 cascade_debug_colors[NUM_CASCADES] = vec3[](vec3(1.0, 0.25, 0.25),
vec3(0.25, 1.0, 0.25),
vec3(0.25, 0.25, 1.0),
vec3(0.41, 0.01, 0.98));

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

float GetSquareFalloffAttenuation(vec3 pos_to_light, float light_inv_radius)
{
    float distance_square = dot(pos_to_light, pos_to_light);
    float factor          = distance_square * light_inv_radius * light_inv_radius;
    float smooth_factor   = max(1.0 - factor * factor, 0.0);

    return (smooth_factor * smooth_factor) / max(distance_square, 1e-5);
}

vec3 CalculatePointLightPBR(PointLight light, vec3 FragPos, vec3 CamPos, vec3 Albedo, vec3 Normal, float Metallic, float Roughness, vec3 F0) {
    //---------------------------------------> View direction (V)
    vec3 V = normalize(CamPos - FragPos);
    vec3 L = normalize(light.position - FragPos);
    vec3 H = normalize(V + L); // Halfway
    //----------------------------------------> Fresnel
    vec3 F = FresnelSchlick(max(dot(H, V), 0.0), F0);
    //----------------------------------------> Cook-Torrance BRDF
    float NDF = DistributionGGX(Normal, H, Roughness);
    float G   = GeometrySmith(Normal, V, L, Roughness);

    vec3 Nominator    = NDF * G * F;
    float Denominator = 4 * max(dot(Normal, V), 0.0) * max(dot(Normal, L), 0.0) + 0.001;
    vec3 Specular	  = Nominator / Denominator;

    vec3 kS = F;
    vec3 kD = 1.0 - kS;
    kD *= 1.0 - Metallic;
    
    float NDotL = max(dot(Normal, L), 0.0);
    vec3  light_direction = light.position - FragPos;
    float attenuation     = GetSquareFalloffAttenuation(light_direction, 1.0 / max(light.radius, 1e-5));
    vec3 Radiance = light.color * light.intensity;
    
    return ((kD * Albedo / PI + Specular) * Radiance * NDotL) * attenuation;
}

vec3 CalculateDirectionalLightPBR(DirectionalLight light, vec3 FragPos, vec3 CamPos, vec3 Albedo, vec3 Normal, float Metallic, float Roughness, vec3 F0) {
    //---------------------------------------> View direction (V)
    vec3 V = normalize(CamPos - FragPos);
    vec3 L = normalize(-light.direction);
    vec3 H = normalize(V + L); // Halfway
    //----------------------------------------> Fresnel
    vec3 F = FresnelSchlick(max(dot(H, V), 0.0), F0);
    //----------------------------------------> Cook-Torrance BRDF
    float NDF = DistributionGGX(Normal, H, Roughness);
    float G   = GeometrySmith(Normal, V, L, Roughness);

    vec3 Nominator    = NDF * G * F;
    float Denominator = 4 * max(dot(Normal, V), 0.0) * max(dot(Normal, L), 0.0) + 0.001;
    vec3 Specular	  = Nominator / Denominator;

    vec3 kS = F;
    vec3 kD = 1.0 - kS;
    kD *= 1.0 - Metallic;

    float NDotL = max(dot(Normal, L), 0.0);
    vec3 Radiance = light.color * light.intensity;

    return (kD * Albedo / PI + Specular) * Radiance * NDotL;
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

vec3 CalculateLo(vec3 L, vec3 N, vec3 V, vec3 Ra, vec3 F0, float R, float M, vec3 A)
{
    vec3 H = normalize(V + L); //Halfway Vector

    //Cook-Torrance BRDF
    float D = DistributionGGX(N, H, R);
    float G = GeometrySmith(N, V, L, R);
    vec3  F = FresnelSchlick(max(dot(H, V), 0.0), F0);

    vec3 Nominator    = D * G * F;
    float Denominator = 4 * max(dot(N, V), 0.0) * max(dot(N, L), 0.0) + 0.001;
    vec3 Specular	  = Nominator / Denominator;

    vec3 Ks = F;
    vec3 Kd = vec3(1.0) - Ks;
    Kd *= 1.0 - M;

    float NDotL = max(dot(N, L), 0.0);
    return (Kd * A / PI + Specular) * Ra * NDotL;
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
    float Shadow = 0.0;
    Shadow = texture(gShadowMap, TexCoords).g;

    vec3 V = normalize(CamPos - FragPos);
    vec3 R = reflect(-V, N);
    
    vec3 F0 = vec3(0.04);
    F0 = mix(F0, Albedo, Metallic);

    vec3 Lighting = vec3(0);
    Lighting += CalculateDirectionalLightPBR(lights.directionalLight, FragPos, CamPos, Albedo, N, Metallic, Roughness, F0);
    for (uint i = 0; i < 1; i++){
          //Lighting += CalculatePointLightPBR(lights.pointLight, FragPos, CamPos, Albedo, N, Metallic, Roughness, F0);
        vec3 L = normalize(lights.pointLight.position - FragPos);
        float distance = length(lights.pointLight.position - FragPos);
        float attenuation = 1.0/(distance * distance);
        vec3 Ra = lights.pointLight.color * attenuation;
        Lighting += CalculateLo(L, N, V, Ra, F0, Roughness, Metallic, Albedo);
    }
    // Indirect IBL Lighting
    vec3 IndirectLighthing = CalculateIndirectLighting(FragPos, CamPos, Albedo, N, Metallic, Roughness, AO);

    vec3 F = FresnelSchlickRoughness(max(dot(N, V), 0.0), F0, Roughness);

    vec3 Ks = F;
    vec3 Kd = 1.0 - Ks;
    Kd *= 1.0 - Metallic;

    vec3 irradiance = texture(gIrradiance, N).rgb;
    vec3 diffuse    = irradiance * Albedo;

    const float MAX_REFLECTION_LOD = 4.0;
    vec3 prefilteredColor = textureLod(gPrefilter, R,  Roughness * MAX_REFLECTION_LOD).rgb;
    vec2 BRDF  = texture(gBRDFLut, vec2(max(dot(N, V), 0.0), Roughness)).rg;
    vec3 specular = prefilteredColor * (F * BRDF.x + BRDF.y);

    vec3 ambient = (Kd * diffuse + specular) * AO;

    vec3 result = vec3(0);
    result =  Shadow * (Lighting + ambient);

    // HDR tonemapping
    result = result / (result + vec3(1.0));
    // gamma correct
    result = pow(result, vec3(1.0/2.2));

    // PCSS Debug
    vec3 cascade_debug_indicator = vec3(0.0, 0.0, 0.0);
    uint cascade_index = 0;

    vec3 viewPos = vec3(camera.u_ViewMatrix * vec4(FragPos, 1.0));
    for (uint i = 0; i < NUM_CASCADES - 1; ++i)
    {
        if (viewPos.z < u_cascade_splits[i])
        {
            cascade_index = i + 1;
        }
    }
    cascade_debug_indicator = cascade_debug_colors[cascade_index];

    FragColor = vec4(result, Alpha);
}