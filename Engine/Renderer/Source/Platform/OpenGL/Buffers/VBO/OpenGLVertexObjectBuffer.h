#pragma once

#include "Renderer/Buffers/VBO/VertexObjectBuffer.h"

namespace Retro::Renderer
{
    class OpenGLVertexObjectBuffer : public VertexObjectBuffer
    {
    public:
        /* Constructor & Destructor */
        OpenGLVertexObjectBuffer(const float* vertices, uint32_t size);
        ~OpenGLVertexObjectBuffer() override;

        void Bind() override;
        void UnBind() override;
    };
}
