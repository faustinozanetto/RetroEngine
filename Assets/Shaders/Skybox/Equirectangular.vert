#version 460
layout (location = 0) in vec3 aPos;
layout (location = 1) in vec2 aTexCoord;
layout (location = 2) in vec3 aNormal;

layout(location = 0) out vec3 LocalPos;

// Camera UB
uniform mat4 projection;
uniform mat4 view;

void main()
{
    LocalPos = aPos;  
    gl_Position =  projection * view * vec4(LocalPos, 1.0);
}