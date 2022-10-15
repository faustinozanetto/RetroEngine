#version 460

const float PI = 3.14159265359;

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

layout(location = 0) out vec4 FragColor;

// Approximates the amount of microfacets that are properly aligned with the halfway vector, thus determines the strength and area for specular light
float NormalDistributionGGX(vec3 normal, vec3 halfway, float roughness) {
	float a = roughness * roughness;
	float a2 = a * a;
	float normDotHalf = dot(normal, halfway);
	float normDotHalf2 = normDotHalf * normDotHalf;

	float numerator = a2;
	float denominator = normDotHalf2 * (a2 - 1.0) + 1.0;
	denominator = PI * denominator * denominator;

	return numerator / denominator;
}

float GeometrySchlickGGX(float cosTheta, float roughness) {
	float r = (roughness + 1.0);
	float k = (roughness * roughness) / 8.0;

	float numerator = cosTheta;
	float denominator = cosTheta * (1.0 - k) + k;

	return numerator / max(denominator, 0.001);
}

// Calculates the amount of specular light. Since diffuse(refraction) and specular(reflection) are mutually exclusive, 
// we can also use this to determine the amount of diffuse light
// Taken from UE4's implementation which is faster and basically identical to the usual Fresnel calculations: https://blog.selfshadow.com/publications/s2013-shading-course/karis/s2013_pbs_epic_notes_v2.pdf
vec3 FresnelSchlick(float cosTheta, vec3 baseReflectivity) {
	return max(baseReflectivity + (1.0 - baseReflectivity) * pow(2, (-5.55473 * cosTheta - 6.98316) * cosTheta), 0.0);
}

// Approximates the geometry obstruction and geometry shadowing respectively, on the microfacet level
float GeometrySmith(vec3 normal, vec3 viewDir, vec3 lightDir, float roughness) {
	return GeometrySchlickGGX(max(dot(normal, viewDir), 0.0), roughness) * GeometrySchlickGGX(max(dot(normal, lightDir), 0.0), roughness);
}

void main() {
  vec3 FragPos = texture(gPosition, TexCoords).rgb;
  vec3 Albedo = texture(gAlbedo, TexCoords).rgb;
  vec3 Normal = texture(gNormal, TexCoords).rgb;

  vec3 FragToView = normalize(camera.u_Position.xyz - FragPos);
  vec3 ReflectionVector = reflect(-FragToView, Normal);

  vec3 BaseReflectivity = vec3(0.04);
  BaseReflectivity = mix(BaseReflectivity, Albedo, 0.5);

  vec3 pointLightIrradiance = vec3(0.0);
  vec3 fragToLight = normalize(lights.pointLight.position.xyz - FragPos);
  vec3 halfway = normalize(FragToView + fragToLight);
  float fragToLightDistance = length(lights.pointLight.position.xyz - FragPos);
  
  // Attenuation calculation (based on Epic's UE4 falloff model)
  float d = fragToLightDistance / 30.0;
  float d2 = d * d;
  float d4 = d2 * d2;
  float falloffNumerator = clamp(1.0 - d4, 0.0, 1.0);
  float attenuation = (falloffNumerator * falloffNumerator) / ((fragToLightDistance * fragToLightDistance) + 1.0);
  vec3 radiance = lights.pointLight.color.rgb * 10 * attenuation;

  // Cook-Torrance Specular BRDF calculations
  float normalDistribution = NormalDistributionGGX(Normal, halfway, 0.5);
  vec3 fresnel = FresnelSchlick(max(dot(halfway, FragToView), 0.0), BaseReflectivity);
  float geometry = GeometrySmith(Normal, FragToView, fragToLight, 0.5);

  // Calculate reflected and refracted light respectively, and since metals absorb all refracted light, we nullify the diffuse lighting based on the metallic parameter
  vec3 specularRatio = fresnel;
  vec3 diffuseRatio = vec3(1.0) - specularRatio;
  diffuseRatio *= 1.0 - 0.5;

  // Finally calculate the specular part of the Cook-Torrance BRDF (max 0.1 stops any visual artifacts)
  vec3 numerator = specularRatio * normalDistribution * geometry;
  float denominator = 4 * max(dot(FragToView, Normal), 0.1) * max(dot(fragToLight, Normal), 0.0) + 0.001; // Prevents any division by zero
  vec3 specular = numerator / denominator;

  // Also calculate the diffuse, a lambertian calculation will be added onto the final radiance calculation
  vec3 diffuse = diffuseRatio * Albedo / PI;
  
  // Add the light's radiance to the irradiance sum
  pointLightIrradiance += (diffuse + specular) * radiance * max(dot(Normal, fragToLight), 0.0);

  	// Calcualte ambient IBL for both diffuse and specular
	vec3 Ambient = vec3(0.05) * Albedo;
	vec3 SpecularRatio = FresnelSchlick(max(dot(Normal, FragToView), 0.0), BaseReflectivity);
	vec3 DiffuseRatio = vec3(1.0) - SpecularRatio;
	DiffuseRatio *= 1.0 - 0.5;

	FragColor = vec4((Albedo * DiffuseRatio) + pointLightIrradiance, 1.0);
}