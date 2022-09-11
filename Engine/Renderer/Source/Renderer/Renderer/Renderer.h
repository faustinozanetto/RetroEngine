#pragma once

#include "Renderer/Renderer/RendererAPI.h"
#include "glm/glm.hpp"

namespace Retro {
	class Renderer {
	public:
		/* Methods */
		static bool Initialize(RenderingAPIType renderingAPIType);
		
		static void SetClearColor(glm::vec4 color);

	private:
		static Scope<RenderingAPI> s_RenderingAPI;
	};
}