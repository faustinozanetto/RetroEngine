#version 460

layout (location = 0) in vec3 aPos;
layout (location = 1) in vec2 aTexCoord;
layout (location = 2) in vec3 aNormal;
layout (location = 3) in vec3 aTangent;
layout (location = 4) in vec3 aBiTangent;

uniform mat4 uTransform;

layout(binding = 0) uniform Camera
{
    mat4 u_ViewProjectionMatrix;
    mat4 u_ViewMatrix;
    mat4 u_ProjectionMatrix;
} camera;

struct GeometryOutput {
    vec2 texCoords;
    vec3 position;
    vec3 normal;
    mat3 tbn;
};

layout (location = 0) out GeometryOutput geometryOutput;

void main() {
    gl_Position = camera.u_ViewProjectionMatrix * uTransform * vec4(aPos, 1.0);

    geometryOutput.position = vec3(uTransform * vec4(aPos, 1.0));
    geometryOutput.texCoords = aTexCoord;
    geometryOutput.normal = mat3(uTransform) * aNormal;
    mat3 normalMatrix = transpose(inverse(mat3(uTransform)));
    vec3 T = normalize(normalMatrix * aTangent);
    vec3 N = normalize(normalMatrix * aNormal);
    T = normalize(T - dot(T, N) * N);
    vec3 B = cross(N, T);
    geometryOutput.tbn = mat3(T, B, N);
}