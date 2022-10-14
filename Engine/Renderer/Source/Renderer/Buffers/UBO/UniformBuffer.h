#pragma once

#include "Core/Base.h"
#include "Renderer/Renderer/GraphicsObject.h"

namespace Retro::Renderer
{
    class UniformBuffer : public GraphicsObject
    {
    public:
        /* Destructor */
        ~UniformBuffer() override;

        /* Methods */
        virtual void SetData(const void* data, uint32_t size, uint32_t offset = 0) = 0;

        /* Instantiate */
        static Ref<UniformBuffer> Create(uint32_t size, uint32_t binding);
    };
}
