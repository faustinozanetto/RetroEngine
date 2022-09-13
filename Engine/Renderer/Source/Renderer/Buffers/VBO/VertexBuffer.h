#pragma once

#include "Core/Base.h"
#include "Renderer/Renderer/GraphicsObject.h"

namespace Retro::Renderer
{
    class VertexBuffer : public GraphicsObject
    {
    public:
        /* Destructor */
        ~VertexBuffer() override;
        
        /* Instantiate */
        static Ref<VertexBuffer> Create(float* data, uint32_t size);
    };
}
