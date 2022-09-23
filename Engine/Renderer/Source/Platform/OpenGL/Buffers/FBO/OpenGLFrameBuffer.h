#pragma once
#include "Renderer/Buffers/FBO/FrameBuffer.h"

namespace Retro::Renderer
{
    class OpenGLFrameBuffer : public FrameBuffer
    {
    public:
        /* Constructor & Destructor */
        OpenGLFrameBuffer(const FFrameBufferSpecification& frameBufferSpecification);
        ~OpenGLFrameBuffer() override;

        /* Methods */
        void Bind() override;
        void UnBind() override;

        uint32_t GetHeight() const override;
        uint32_t GetWidth() const override;

        void Resize(uint32_t newWidth, uint32_t newHeight) override;

    private:
        FFrameBufferSpecification m_FrameBufferSpecification;
    };
}
