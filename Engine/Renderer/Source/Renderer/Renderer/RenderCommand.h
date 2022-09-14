#pragma once

#include "Renderer/Buffers/VAO/VertexArrayBuffer.h"
#include "Renderer/Shader/Shader.h"

namespace Retro::Renderer
{
    struct RenderCommand
    {
        Ref<Shader> shader;
        Ref<VertexArrayBuffer> vao;
        glm::mat4 transform;

        RenderCommand(const Ref<Shader>& shader, const Ref<VertexArrayBuffer>& vao,
                      const glm::mat4& transform) : shader(shader), vao(vao), transform(transform)
        {
        }
    };
}
