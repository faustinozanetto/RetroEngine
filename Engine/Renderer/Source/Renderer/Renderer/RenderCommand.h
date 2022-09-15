#pragma once

#include "Renderer/Buffers/VAO/VertexArrayBuffer.h"
#include "Renderer/Shader/Shader.h"
#include "Renderer/Textures/Texture.h"

namespace Retro::Renderer
{
    struct RenderCommand
    {
        Ref<Shader> shader;
        Ref<VertexArrayBuffer> vao;
        Ref<Texture> texture;
        glm::mat4 transform;

        RenderCommand(const Ref<Shader>& shader, const Ref<VertexArrayBuffer>& vao, const Ref<Texture>& texture,
                      const glm::mat4& transform) : shader(shader), vao(vao), texture(texture), transform(transform)
        {
        }
    };
}
