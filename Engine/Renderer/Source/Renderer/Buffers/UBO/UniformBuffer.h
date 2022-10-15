#pragma once

#include "Core/Base.h"
#include "Renderer/Renderer/GraphicsObject.h"
#include <Renderer/Buffers/VBO/VertexObjectBuffer.h>

namespace Retro::Renderer
{
    class UniformBuffer : public GraphicsObject
    {
    public:
        /* Destructor */
        ~UniformBuffer() override;

        /* Methods */
        virtual void SetData(const void* data, uint32_t size, uint32_t offset = 0) = 0;
        virtual void SetIBOLayout(const VBOLayout& layout, uint32_t block, uint32_t count = 1) = 0;

        /* Instantiate */
        static Ref<UniformBuffer> Create(uint32_t size, uint32_t binding);
    };
}
