#pragma once
#include "Core/Base.h"
#include "Renderer/Renderer/GraphicsObject.h"

namespace Retro::Renderer
{
    enum class EFrameBufferAttachmentFormat
    {
        None = 0,
        Color = 1,
        Depth = 2
    };

    struct FFrameBufferTextureSpecification
    {
        EFrameBufferAttachmentFormat format;

        FFrameBufferTextureSpecification(EFrameBufferAttachmentFormat format) : format(format)
        {
        }
    };

    struct FFrameBufferSpecification
    {
        uint32_t width;
        uint32_t height;

        std::vector<FFrameBufferTextureSpecification> attachments;

        FFrameBufferSpecification() : width(1920), height(1080), attachments({})
        {
        }

        FFrameBufferSpecification(uint32_t width, uint32_t height,
                                  std::initializer_list<FFrameBufferTextureSpecification> attachments) : width(width),
            height(height), attachments(attachments)
        {
        }
    };

    class FrameBuffer : public GraphicsObject
    {
    public:
        /* Destructor */
        ~FrameBuffer() override;

        /* Methods */
        void Bind() override = 0;
        void UnBind() override = 0;

        virtual void AddTextureAttachment(const FFrameBufferTextureSpecification& frameBufferTextureAttachment) = 0;

        virtual void Resize(uint32_t newWidth, uint32_t newHeight) = 0;
        virtual uint32_t GetRendererID() = 0;
        virtual uint32_t GetWidth() const = 0;
        virtual uint32_t GetHeight() const = 0;

        /* Instantiate */
        static Ref<FrameBuffer> Create(const FFrameBufferSpecification& frameBufferSpecification);
    };
}
