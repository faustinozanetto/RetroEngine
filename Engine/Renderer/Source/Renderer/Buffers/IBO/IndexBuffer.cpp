#include "pch.h"

#include "IndexBuffer.h"
#include "Platform/OpenGL/Buffers/IBO/OpenGLIndexBuffer.h"
#include "Renderer/Renderer/RendererContext.h"
#include "Renderer/Renderer/Renderer.h"

namespace Retro::Renderer
{
	IndexBuffer::~IndexBuffer()
	{
	}

	Shared<IndexBuffer> IndexBuffer::Create(uint32_t* data, uint32_t size)
	{
		switch (Renderer::GetRenderingAPIType())
		{
		case RenderingAPIType::None:
			{
				Logger::Error("IndexBuffer::Create | Unknown rendering api!.");
				return {};
			}
		case RenderingAPIType::OpenGL:
			{
				return CreateShared<OpenGLIndexBuffer>(data, size);
			}
		}
		return {};
	}
}
