#include "pch.h"

#include "VertexArrayBuffer.h"

#include "Platform/OpenGL/Buffers/VAO/OpenGLVertexArrayBuffer.h"
#include "Renderer/Renderer/RendererContext.h"
#include "Renderer/Renderer/Renderer.h"

namespace Retro::Renderer
{
	VertexArrayBuffer::VertexArrayBuffer()
	{
	}

	VertexArrayBuffer::~VertexArrayBuffer()
	{
	}

	Ref<VertexArrayBuffer> VertexArrayBuffer::Create()
	{
		switch (Renderer::GetRenderingAPIType())
		{
		case RenderingAPIType::None:
			{
				Logger::Error("VertexArrayBuffer::Create | Unknown rendering api!.");
				return {};
			}
		case RenderingAPIType::OpenGL:
			{
				return CreateRef<OpenGLVertexArrayBuffer>();
			}
		}
		return {};
	}
}
