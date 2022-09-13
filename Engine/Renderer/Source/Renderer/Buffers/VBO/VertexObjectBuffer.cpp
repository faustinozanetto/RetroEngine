#include "pch.h"
#include "VertexObjectBuffer.h"

#include "Platform/OpenGL/Buffers/VBO/OpenGLVertexObjectBuffer.h"
#include "Renderer/Renderer/RendererContext.h"
#include "Renderer/Renderer/Renderer.h"

namespace Retro::Renderer
{
    VertexObjectBuffer::~VertexObjectBuffer()
    {
    }

    Ref<VertexObjectBuffer> VertexObjectBuffer::Create(float* data, uint32_t size)
    {
        switch (Renderer::GetRenderingAPIType())
        {
        case RenderingAPIType::None:
            {
                Logger::Error("VertexObjectBuffer::Create | Unknown rendering api!.");
                return nullptr;
            }
        case RenderingAPIType::OpenGL:
            {
                return CreateRef<OpenGLVertexObjectBuffer>(data, size);
            }
        }
        return {};
    }
}
