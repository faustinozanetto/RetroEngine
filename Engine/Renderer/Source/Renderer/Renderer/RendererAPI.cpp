#include "pch.h"

#include "Renderer/Renderer/RendererAPI.h"
#include "Platform/OpenGL/Renderer/OpenGLRenderingAPI.h"

namespace Retro {
	Scope<RenderingAPI> RenderingAPI::Create(RenderingAPIType renderingAPITYpe)
	{
		switch (renderingAPITYpe) {
		case RenderingAPIType::None: {
			Logger::Error("RenderingAPI::Create | Unknown rendering api!.");
			return nullptr;
		}
		case RenderingAPIType::OpenGL: {
			return CreateScope<OpenGLRenderingAPI>();
		}
		}
	}
}