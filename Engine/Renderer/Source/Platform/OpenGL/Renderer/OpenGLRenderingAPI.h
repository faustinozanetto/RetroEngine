#pragma once

#include "glad/glad.h"
#include "Renderer/Renderer/RendererAPI.h"
#include "glm/glm.hpp"

namespace Retro::Renderer
{
	class OpenGLRenderingAPI : public RenderingAPI
	{
	public:
		/* Constructor & Destructor */
		OpenGLRenderingAPI();
		~OpenGLRenderingAPI() override;

		/* Methods */
		bool Initialize() override;
		void SetClearColor(glm::vec4 color) override;
		void SetRenderMode(ERenderMode renderMode) override;
		void SetRendererState(ERendererState renderState, bool enabled) override;
		void ClearScreen() override;
		void ProcessRendereable(int size) override;
		void BindTexture(uint32_t textureHandle, uint32_t textureSlot) override;
		double GetTime() override;

		static GLenum GetRendererStateToOpenGL(ERendererState renderState);
	};
}
