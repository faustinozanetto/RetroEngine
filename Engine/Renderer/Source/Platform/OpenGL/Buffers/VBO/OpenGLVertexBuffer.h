#pragma once
#include "Renderer/Buffers/VBO/VertexBuffer.h"

namespace Retro::Renderer
{
    class OpenGLVertexBuffer : public VertexBuffer
    {
    public:
        /* Constructor & Destructor */
        OpenGLVertexBuffer(const float* vertices, uint32_t size);
        ~OpenGLVertexBuffer() override;

        void Bind() override;
        void UnBind() override;
    };
}
