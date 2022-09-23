#pragma once
#include "Core/Base.h"
#include "Renderer/Renderer/GraphicsObject.h"

namespace Retro::Renderer
{
    struct FFrameBufferSpecification
    {
        uint32_t width;
        uint32_t height;
    };
    
    class FrameBuffer : public GraphicsObject
    {
    public:
        /* Destructor */
        ~FrameBuffer() override;

        /* Methods */
        void Bind() override = 0;
        void UnBind() override = 0;

        virtual void Resize(uint32_t newWidth, uint32_t newHeight) = 0;
        virtual uint32_t GetWidth() const = 0;
        virtual uint32_t GetHeight() const = 0;

        /* Instantiate */
        static Ref<FrameBuffer> Create(const FFrameBufferSpecification& frameBufferSpecification);
    };
}
