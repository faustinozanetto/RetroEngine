#version 460

layout (location = 0) in vec3 aPos;
layout (location = 1) in vec2 aTexCoord;
layout (location = 2) in vec3 aNormal;

uniform mat4 u_model;

layout(std140, binding = 0) uniform Camera
{
    mat4 u_ViewProjectionMatrix;
    mat4 u_ViewMatrix;
    mat4 u_ProjectionMatrix;
    vec3 u_Position;
} camera;

out Vertex {
  vec3 worldPosition;
  vec3 normalGeometry;
};

void main() {
  worldPosition = vec3(u_model * vec4(aPos,1.0));
  normalGeometry = normalize(mat3(transpose(inverse(u_model))) * aNormal);
  gl_Position = camera.u_ProjectionMatrix * camera.u_ViewMatrix * vec4(aPos, 1.0);
}