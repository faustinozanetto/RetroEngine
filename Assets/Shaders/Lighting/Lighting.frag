#version 460

// Tex coords from vertex shader.
layout(location = 0) in vec2 TexCoords;

uniform vec3 lightPos; 
uniform vec3 lightColor;

//GBuffer samplers
layout(binding = 0) uniform sampler2D gPosition;
layout(binding = 1) uniform sampler2D gNormal;
layout(binding = 2) uniform sampler2D gAlbedo;

layout(location = 0) out vec4 FragColor;

void main() {
  vec3 FragPos = texture(gPosition, TexCoords).rgb;
  vec4 SampledAlbedo = texture(gAlbedo, TexCoords).rgba;
  vec3 Albedo = SampledAlbedo.rgb;
  vec3 Normal = texture(gNormal, TexCoords).rgb;

  // ambient
  float ambientStrength = 0.1;
  vec3 ambient = ambientStrength * lightColor;

  // diffuse 
  vec3 norm = normalize(Normal);
  vec3 lightDir = normalize(lightPos - FragPos);
  float diff = max(dot(norm, lightDir), 0.0);
  vec3 diffuse = diff * lightColor;

  vec3 result = (ambient + diffuse) * Albedo;
  FragColor = vec4(result, 1.0);
}