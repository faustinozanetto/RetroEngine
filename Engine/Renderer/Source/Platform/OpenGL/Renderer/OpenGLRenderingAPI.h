#pragma once

#include "Renderer/Renderer/RendererAPI.h"

namespace Retro {
	class OpenGLRenderingAPI : public RenderingAPI {
	public:
		/* Constructor & Destructor */
		OpenGLRenderingAPI();
		~OpenGLRenderingAPI() override;

		/* Methods */
		bool Initialize() override;
	};
}