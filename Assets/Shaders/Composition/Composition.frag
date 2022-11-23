#version 460

layout (location = 0) out vec4 FragColor;
layout (location = 0) in vec2 vTexCoord;

layout (binding = 0) uniform sampler2D uShadedTexure;
layout (binding = 1) uniform sampler2D uShadowTexture;

void main()
{
    vec3 Shaded = texture(uShadedTexure, vTexCoord).rgb;
    float Shadow = texture(uShadowTexture, vTexCoord).r;
    FragColor = vec4(Shadow * Shaded, 1.0);
}