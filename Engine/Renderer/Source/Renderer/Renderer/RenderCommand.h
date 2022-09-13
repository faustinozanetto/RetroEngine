#pragma once
#include "Renderer/Buffers/VBO/VertexBuffer.h"

namespace Retro::Renderer
{
    struct RenderCommand
    {
        Ref<VertexBuffer> vbo;
    };
}
