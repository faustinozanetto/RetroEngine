#pragma once

#include "Renderer/Renderer/RendererAPI.h"
#include "glm/glm.hpp"

namespace Retro {
	class OpenGLRenderingAPI : public RenderingAPI {
	public:
		/* Constructor & Destructor */
		OpenGLRenderingAPI();
		~OpenGLRenderingAPI() override;

		/* Methods */
		bool Initialize() override;
		void SetClearColor(glm::vec4 color) override;
		void ClearScreen() override;
	};
}