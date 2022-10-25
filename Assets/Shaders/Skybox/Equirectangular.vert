#version 460

layout (location = 0) in vec3 aPos;
layout (location = 1) in vec2 aTexCoord;
layout (location = 2) in vec3 aNormal;

layout(location = 0) out vec3 localPos;

// Camera UB
uniform Camera
{
  mat4 u_ViewMatrix;
  mat4 u_ProjectionMatrix;
} camera;

void main()
{
    localPos = aPos;  
    gl_Position =  camera.u_ProjectionMatrix * camera.u_ViewMatrix * vec4(localPos, 1.0);
}