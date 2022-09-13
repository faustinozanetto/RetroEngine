#pragma once

#include "Core/Base.h"
#include "Renderer/Renderer/RendererAPI.h"
#include "Renderer/Renderer/RendererContext.h"
#include "glm/glm.hpp"

namespace Retro {
	class Renderer {
	public:
		/* Methods */
		static bool Initialize(RenderingAPIType renderingAPIType);
		static void SetRendererContext(RendererContext* context);
		
		static void SetClearColor(glm::vec4 color);
		static void ClearScreen();
		static void SwapBuffers();
		static void PollInput();

		/* Getters */
		static RenderingAPI* GetRenderingAPI();

	private:
		static Scope<RenderingAPI> s_RenderingAPI;
		static RendererContext* s_Context;
	};
}