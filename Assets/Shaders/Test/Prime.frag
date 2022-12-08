#version 460

uniform vec2 uResolution;
uniform vec2 uMousePos;
uniform float uTime;

layout (location = 0) in vec2 vTexCoord;
layout (location = 0) out vec4 FragColor;

bool isPrimeNumber(int num) {
  if (num == 2) return true;
  if (num < 2 || num % 2 == 0) return false;
  for (int i = 3; i <= int(sqrt(float(num))); i+=2) {
    if (num % i == 0) return false;
  }
  return true;
}

void main()
{
  float zoom = uMousePos.x / uResolution.x;
  vec2 uv = (floor(gl_FragCoord.xy) - floor(gl_FragCoord.xy * 0.5)) * zoom;
  vec3 color = vec3(0);

  uv.y += floor(uResolution.y * 0.5);
  int num = int(uv.y) * int(uResolution.x) + int(uv.x);
  if (isPrimeNumber(num)) color.g += 1.0;

  FragColor = vec4(color, 1.0);
}