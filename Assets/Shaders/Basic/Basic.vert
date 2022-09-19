#version 460

layout (location = 0) in vec3 aPos;
layout (location = 1) in vec2 aTexCoord;
layout (location = 2) in vec3 aNormal;
layout (location = 3) in vec3 aTangent;
layout (location = 4) in vec3 aBiTangent;

layout (location = 0) out vec3 vPos;
layout (location = 1) out vec2 vTexCoord;
layout (location = 2) out vec3 vNormal;
layout (location = 3) out vec3 vTangent;
layout (location = 4) out vec3 vBiTangent;

uniform mat4 uTransform;

void main() {
    gl_Position = uTransform * vec4(aPos, 1.0);
    vPos = vec3(uTransform * vec4(aPos, 1.0));;
    vTexCoord = aTexCoord;
    vNormal = aNormal;
    vTangent = aTangent;
    vBiTangent = aBiTangent;
}