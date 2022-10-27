#version 460

layout(location = 0) out vec4 FragColor;
layout(location = 0) in  vec3 WorldPos;

layout(binding = 0) uniform samplerCube skybox;

void main()
{    
    vec3 envColor = textureLod(skybox, WorldPos, 1.0).rgb * 1.0;

    // HDR tonemap and gamma correct
    vec3 mapped = vec3(1.0) - exp(-envColor * 0.7);
    // gamma correction 
	mapped = pow(mapped, vec3(1.0 / 1.9));
    
    FragColor = vec4(mapped, 1.0);
}