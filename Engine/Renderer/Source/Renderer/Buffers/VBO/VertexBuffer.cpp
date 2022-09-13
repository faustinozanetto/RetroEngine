#include "pch.h"
#include "VertexBuffer.h"

#include "Platform/OpenGL/Buffers/VBO/OpenGLVertexBuffer.h"
#include "Renderer/Renderer/RendererContext.h"
#include "Renderer/Renderer/Renderer.h"

namespace Retro::Renderer
{
    VertexBuffer::~VertexBuffer()
    {
    }

    Ref<VertexBuffer> VertexBuffer::Create(float* data, uint32_t size)
    {
        switch (Renderer::GetRenderingAPIType())
        {
        case RenderingAPIType::None:
            {
                Logger::Error("VertexBuffer::Create | Unknown rendering api!.");
                return nullptr;
            }
        case RenderingAPIType::OpenGL:
            {
                return CreateScope<OpenGLVertexBuffer>(data, size);
            }
        }
        return {};
    }
}
