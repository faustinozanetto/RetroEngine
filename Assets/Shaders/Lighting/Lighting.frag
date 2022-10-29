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


float DistributionGGX(vec3 N, vec3 H, float roughness)
{
    float a = roughness*roughness;
    float a2 = a*a;
    float NdotH = max(dot(N, H), 0.0);
    float NdotH2 = NdotH*NdotH;

    float nom   = a2;
    float denom = (NdotH2 * (a2 - 1.0) + 1.0);
    denom = PI * denom * denom;

    return nom / denom;
}
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
vec3 fresnelSchlick(float cosTheta, vec3 F0)
{
    return F0 + (1.0 - F0) * pow(clamp(1.0 - cosTheta, 0.0, 1.0), 5.0);
}
// ----------------------------------------------------------------------------
vec3 fresnelSchlickRoughness(float cosTheta, vec3 F0, float roughness)
{
    return F0 + (max(vec3(1.0 - roughness), F0) - F0) * pow(clamp(1.0 - cosTheta, 0.0, 1.0), 5.0);
}   

void main() {
	vec3 FragPos = texture(gPosition, TexCoords).rgb;
	vec3 N = texture(gNormal, TexCoords).rgb;
	vec3 Albedo = pow(texture(gAlbedo, TexCoords).rgb, vec3(2.2));
	float Roughness = texture(gRoughMetalAO,  TexCoords).r;
	float Metallic  = texture(gRoughMetalAO,  TexCoords).g;
    float AO		= texture(gRoughMetalAO,  TexCoords).b;
  
	vec3 V = normalize(camera.u_Position.rgb - FragPos);
    vec3 R = reflect(-V, N); 

    // calculate reflectance at normal incidence; if dia-electric (like plastic) use F0 
    // of 0.04 and if it's a metal, use the albedo color as F0 (metallic workflow)    
    vec3 F0 = vec3(0.04); 
    F0 = mix(F0, Albedo, Metallic);

    // reflectance equation
    vec3 Lo = vec3(0.0);
    {
        // calculate per-light radiance
        vec3 L = normalize(lights.pointLight.position - FragPos);
        vec3 H = normalize(V + L);
        float distance = length(lights.pointLight.position - FragPos);
        float attenuation = 1.0 / (distance * distance);
        vec3 radiance = lights.pointLight.color * attenuation;

        // Cook-Torrance BRDF
        float NDF = DistributionGGX(N, H, Roughness);   
        float G   = GeometrySmith(N, V, L, Roughness);    
        vec3 F    = fresnelSchlick(max(dot(H, V), 0.0), F0);        
        
        vec3 numerator    = NDF * G * F;
        float denominator = 4.0 * max(dot(N, V), 0.0) * max(dot(N, L), 0.0) + 0.0001; // + 0.0001 to prevent divide by zero
        vec3 specular = numerator / denominator;
        
            // kS is equal to Fresnel
        vec3 kS = F;
        // for energy conservation, the diffuse and specular light can't
        // be above 1.0 (unless the surface emits light); to preserve this
        // relationship the diffuse component (kD) should equal 1.0 - kS.
        vec3 kD = vec3(1.0) - kS;
        // multiply kD by the inverse metalness such that only non-metals 
        // have diffuse lighting, or a linear blend if partly metal (pure metals
        // have no diffuse light).
        kD *= 1.0 - Metallic;	                
            
        // scale light by NdotL
        float NdotL = max(dot(N, L), 0.0);        

        // add to outgoing radiance Lo
        Lo += (kD * Albedo / PI + specular) * radiance * NdotL; // note that we already multiplied the BRDF by the Fresnel (kS) so we won't multiply by kS again
    }

    // ambient lighting (we now use IBL as the ambient term)
    vec3 F = fresnelSchlickRoughness(max(dot(N, V), 0.0), F0, Roughness);
    
    vec3 kS = F;
    vec3 kD = 1.0 - kS;
    kD *= 1.0 - Metallic;	  
    
    vec3 irradiance = texture(gIrradiance, N).rgb;
    vec3 diffuse      = irradiance * Albedo;
    
    // sample both the pre-filter map and the BRDF lut and combine them together as per the Split-Sum approximation to get the IBL specular part.
    const float MAX_REFLECTION_LOD = 4.0;
    vec3 prefilteredColor = textureLod(gPrefilter, R,  Roughness * MAX_REFLECTION_LOD).rgb;    
    vec2 brdf  = texture(gBRDFLut, vec2(max(dot(N, V), 0.0), Roughness)).rg;
    vec3 specular = prefilteredColor * (F * brdf.x + brdf.y);

    vec3 ambient = (kD * diffuse + specular) * AO;
    
    vec3 color = ambient + Lo;

    // HDR tonemapping
    color = color / (color + vec3(1.0));
    // gamma correct
    color = pow(color, vec3(1.0/2.2)); 

    FragColor = vec4(color , 1.0);
}