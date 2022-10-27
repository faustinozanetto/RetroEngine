#version 460
layout (location = 0) in vec3 aPos;
layout (location = 1) in vec3 aNormal;
layout (location = 2) in vec2 aTexCoord;

layout(location = 0) out vec3 worldPos;

uniform mat4 projection;
uniform mat4 view;

void main()
{
    worldPos = aPos;  
    gl_Position = projection * view * vec4(worldPos, 1.0);
}
