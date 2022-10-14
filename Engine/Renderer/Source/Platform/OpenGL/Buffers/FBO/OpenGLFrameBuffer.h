#pragma once
#include "glad/glad.h"
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

        void AddColorTextureAttachment(
            const FFrameBufferColorTextureSpecification& frameBufferColorTextureAttachment) override;

        uint32_t GetColorAttachmentID(uint32_t slot) override;
        uint32_t GetDepthAttachmentID() override;
        uint32_t GetHeight() const override;
        uint32_t GetWidth() const override;

        void Resize(uint32_t newWidth, uint32_t newHeight) override;

    private:
        void Reconstruct();

        void GenerateColorTexture(uint32_t textureHandle, int index, uint32_t width, uint32_t height, GLenum format,
                                  GLenum dataFormat);

        void GenerateDepthTexture(uint32_t textureHandle, uint32_t width, uint32_t height);

    private:
        FFrameBufferSpecification m_FrameBufferSpecification;
        std::vector<FFrameBufferColorTextureSpecification> m_ColorTextureAttachmentsSpecifications;
        std::vector<uint32_t> m_ColorTextureAttachments;
        uint32_t m_DepthTextureAttachment;
    };
}
