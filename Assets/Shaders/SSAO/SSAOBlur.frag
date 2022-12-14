#version 460

layout (location = 0) in vec2 vTexCoord;

layout(binding = 0) uniform sampler2D gInputTexture;

layout (location = 0) out float FragColor;

void main() {
  vec2 texelSize = 1.0 / vec2(textureSize(gInputTexture, 0));
  float result = 0.0;

  for (int x = -2; x < 2; ++x) 
    {
        for (int y = -2; y < 2; ++y) 
        {
            vec2 offset = vec2(float(x), float(y)) * texelSize;
            result += texture(gInputTexture, vTexCoord + offset).r;
        }
    }
    FragColor = result / (4.0 * 4.0);
}