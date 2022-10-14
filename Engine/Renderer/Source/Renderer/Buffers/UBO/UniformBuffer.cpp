#include "pch.h"

#include "UniformBuffer.h"

#include "Platform/OpenGL/Buffers/UBO/OpenGLUniformBuffer.h"
#include "Renderer/Renderer/RendererContext.h"
#include "Renderer/Renderer/Renderer.h"

namespace Retro::Renderer
{
    UniformBuffer::~UniformBuffer()
    {
    }

    Ref<UniformBuffer> UniformBuffer::Create(uint32_t size, uint32_t binding)
    {
        switch (Renderer::GetRenderingAPIType())
        {
        case RenderingAPIType::None:
            {
                Logger::Error("UniformBuffer::Create | Unknown rendering api!.");
                return {};
            }
        case RenderingAPIType::OpenGL:
            {
                return CreateRef<OpenGLUniformBuffer>(size, binding);
            }
        }
        return {};
    }
}