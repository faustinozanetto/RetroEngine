#include "pch.h"

#include "Platform/OpenGL/Renderer/OpenGLRendererContext.h"
#include "Renderer/Renderer/RendererContext.h"
#include "Renderer/Renderer/Renderer.h"
#include "GLFW/glfw3.h"

namespace Retro::Renderer
{
	Scope<RendererContext> RendererContext::Create(void* windowHandle)
	{
		switch (Renderer::GetRenderingAPIType())
		{
		case RenderingAPIType::None:
			{
				Logger::Error("RendererContext::Create | Unknown rendering api!.");
				return nullptr;
			}
		case RenderingAPIType::OpenGL:
			{
				return CreateScope<OpenGLRendererContext>(static_cast<GLFWwindow*>(windowHandle));
			}
		}
		return {};
	}
}
