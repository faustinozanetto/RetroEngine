#pragma once

#include "Core/Base.h"
#include "Renderer/Renderer/RendererAPI.h"
#include "Renderer/Renderer/RendererContext.h"
#include "Renderer/Window/Window.h"
#include "glm/glm.hpp"

namespace Retro::Renderer {
	class Renderer {
	public:
		/* Methods */
		static bool Initialize(RenderingAPIType renderingAPIType, const Window& window);
		
		static void SetClearColor(glm::vec4 color);
		static void ClearScreen();
		static void SwapBuffers();
		static void PollInput();

		/* Getters */
		static RenderingAPIType GetRenderingAPIType();

	private:
		static Scope<RenderingAPI> s_RenderingAPI;
		static Scope<RendererContext> s_Context;
	};
}
