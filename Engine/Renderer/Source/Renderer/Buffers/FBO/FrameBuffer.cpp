#include "pch.h"

#include "FrameBuffer.h"

#include "Platform/OpenGL/Buffers/FBO/OpenGLFrameBuffer.h"
#include "Renderer/Renderer/RendererContext.h"
#include "Renderer/Renderer/Renderer.h"

namespace Retro::Renderer
{
	FrameBuffer::~FrameBuffer()
	{
	}

	Shared<FrameBuffer> FrameBuffer::Create(const FFrameBufferSpecification& frameBufferSpecification)
	{
		switch (Renderer::GetRenderingAPIType())
		{
		case RenderingAPIType::None:
			{
				Logger::Error("FrameBuffer::Create | Unknown rendering api!.");
				return {};
			}
		case RenderingAPIType::OpenGL:
			{
				return CreateShared<OpenGLFrameBuffer>(frameBufferSpecification);
			}
		}
		return {};
	}
}
