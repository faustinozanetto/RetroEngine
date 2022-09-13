#pragma once

#include "Core/Base.h"
#include "Renderer/Renderer/GraphicsObject.h"

namespace Retro::Renderer
{
    class VertexObjectBuffer : public GraphicsObject
    {
    public:
        /* Destructor */
        ~VertexObjectBuffer() override;
        
        /* Instantiate */
        static Ref<VertexObjectBuffer> Create(float* data, uint32_t size);
    };
}
