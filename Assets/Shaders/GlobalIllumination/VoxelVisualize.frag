#version 460

layout (location = 0) in vec4 InputColor;

layout (location = 0) out vec4 FragColor;

void main()
{
  float gamma = 2.24;
  vec3 color = InputColor.rgb;
  color = color / (color + 1.0);
  color = pow(color, vec3(gamma));
  FragColor.rgb = color;
}