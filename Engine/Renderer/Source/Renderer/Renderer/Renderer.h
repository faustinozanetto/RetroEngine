#pragma once

#include "Renderer/Renderer/RendererAPI.h"

namespace Retro {
	class Renderer {
	public:
		/* Methods */
		static bool Initialize(RenderingAPIType renderingAPIType);

	private:
		static Scope<RenderingAPI> s_RenderingAPI;
	};
}