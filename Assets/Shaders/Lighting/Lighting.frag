#version 460

// Tex coords from vertex shader.
layout(location = 0) in vec2 TexCoords;

struct PointLight {
  vec3 position;
  vec3 color;
  float constant;
  float linear;
  float quadratic;
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
} lights;

//GBuffer samplers
layout(binding = 0) uniform sampler2D gPosition;
layout(binding = 1) uniform sampler2D gAlbedo;
layout(binding = 2) uniform sampler2D gNormal;
layout(binding = 3) uniform sampler2D gRoughMetalAO;

layout(binding = 4) uniform samplerCube gIrradiance;
layout(binding = 5) uniform samplerCube gPrefilter;
layout(binding = 6) uniform sampler2D gBRDFLut;

layout(location = 0) out vec4 FragColor;

const float PI = 3.14159265359;

// ----------------------------------------------------------------------------
float DistributionGGX(vec3 N, vec3 H, float roughness)
{
    float a = roughness*roughness;
    float a2 = a*a;
    float NdotH = max(dot(N, H), 0.0);
    float NdotH2 = NdotH*NdotH;

    float nom   = a2;
    float denom = (NdotH2 * (a2 - 1.0) + 1.0);
    denom = PI * denom * denom;

    return nom / max(denom, 0.001); // prevent divide by zero for roughness=0.0 and NdotH=1.0
}
///////////////////////////////////////////////////PBR////////////////////////////////////////
// ----------------------------------------------------------------------------
float GeometrySchlickGGX(float NdotV, float roughness)
{
    float r = (roughness + 1.0);
    float k = (r*r) / 8.0;

    float nom   = NdotV;
    float denom = NdotV * (1.0 - k) + k;

    return nom / denom;
}

// ----------------------------------------------------------------------------
float GeometrySmith(vec3 N, vec3 V, vec3 L, float roughness)
{
    float NdotV = max(dot(N, V), 0.0);
    float NdotL = max(dot(N, L), 0.0);
    float ggx2 = GeometrySchlickGGX(NdotV, roughness);
    float ggx1 = GeometrySchlickGGX(NdotL, roughness);

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
	vec3 Albedo = texture(gAlbedo, TexCoords).rgb;
    vec3 Normal = texture(gNormal, TexCoords).rgb;
	float Roughness = texture(gRoughMetalAO,  TexCoords).r;
	float Metallic  = texture(gRoughMetalAO,  TexCoords).g;
    float AO		= texture(gRoughMetalAO,  TexCoords).b;

    //---------------------------------------> View direction (V)
    vec3 V = normalize(camera.u_Position.rgb - FragPos);
    vec3 R = reflect(-V, Normal);
    //----------------------------------------> Specular reflectance at normal incidence
    vec3 F0 = vec3(0.04);
    F0 = mix(F0, Albedo, Metallic);
    float NdotV = clamp(dot(Normal, V), 0., 1.);
    //---------------------------------------> Cook-Torrance reflectance equation
    vec3 Lo = vec3(0);
    
    // Per lights
    for(uint i = 0; i < 1; i++){
        //reflectance += calcPointLight(1, Normal, FragPos, V, Albedo, Roughness, Metallic, F0, ViewDistance);
        vec3 L = normalize(lights.pointLight.position.rgb - FragPos);
        float distance = length(lights.pointLight.position.rgb - FragPos);
        float attenuation = 1.0/(distance * distance);
        vec3 Ra = lights.pointLight.color.rgb * attenuation;
        Lo += CalculateLo(L, Normal, V, Ra, F0, Roughness, Metallic, Albedo);
    }

    vec3 F = FresnelSchlickRoughness(max(dot(Normal, V), 0.0), F0, Roughness);

    vec3 Ks = F;
    vec3 Kd = 1.0 - Ks;
    Kd *= 1.0 - Metallic;

    vec3 irradiance = texture(gIrradiance, Normal).rgb * 0.75;
    vec3 diffuse    = irradiance * Albedo;

    const float MAX_REFLECTION_LOD = 4.0;
    vec3 prefilteredColor = textureLod(gPrefilter, R,  Roughness * MAX_REFLECTION_LOD).rgb;
    vec2 BRDF  = texture(gBRDFLut, vec2(max(dot(Normal, V), 0.0), Roughness)).rg;
    vec3 specular = prefilteredColor * (F * BRDF.x + BRDF.y);

    vec3 ambient = (Kd * diffuse + specular) * AO;

    vec3 result = vec3(0);
    result = (1) * Lo + ambient;
    vec3 hdrColor = result;

    // reinhard tone mapping
    vec3 mapped = vec3(1.0) - exp(-hdrColor * 0.65);
    // gamma correction 
    mapped = pow(mapped, vec3(1.0 / 1.9));

    FragColor = vec4(mapped, 1.0);
}