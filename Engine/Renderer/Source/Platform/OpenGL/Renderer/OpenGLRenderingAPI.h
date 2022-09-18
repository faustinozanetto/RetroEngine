#pragma once

#include "Renderer/Renderer/RendererAPI.h"
#include "glm/glm.hpp"

namespace Retro::Renderer {
	class OpenGLRenderingAPI : public RenderingAPI {
	public:
		/* Constructor & Destructor */
		OpenGLRenderingAPI();
		~OpenGLRenderingAPI() override;

		/* Methods */
		bool Initialize() override;
		void SetClearColor(glm::vec4 color) override;
		void ClearScreen() override;
		void ProcessRendereable(int size) override;
		double GetTime() override;
	};
}