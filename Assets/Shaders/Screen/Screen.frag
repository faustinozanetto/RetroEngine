#version 460
layout (location = 0) out vec4 FragColor;
layout (location = 0) in vec2 vTexCoord;
layout(binding = 0) uniform sampler2D uTexture;

void main()
{
    vec3 color = texture(uTexture, vTexCoord).rgb;
    FragColor = vec4(color, 1.0);
}