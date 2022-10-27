#include "pch.h"

#include "Platform/OpenGL/Renderer/OpenGLRendererContext.h"
#include "renderer/renderer/renderer_context.h"
#include "Renderer/Renderer/Renderer.h"
#include "GLFW/glfw3.h"

namespace Retro::Renderer
{
	unique<renderer_context> renderer_context::create(void *window_handle)
	{
		switch (renderer::get_renderer_api_type())
		{
		case renderer_api_type::none:
		{
			logger::error("renderer_context::create | Unknown renderer api!.");
			return nullptr;
		}
		case renderer_api_type::open_gl:
		{
			return create_unique<open_gl_renderer_context>(static_cast<GLFWwindow *>(window_handle));
		}
		}
		return {};
	}
}
