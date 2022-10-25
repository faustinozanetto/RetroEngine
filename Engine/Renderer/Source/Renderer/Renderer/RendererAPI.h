#pragma once

#include "Core/Base.h"
#include "glm/glm.hpp"

namespace Retro::Renderer
{
	enum class RenderingAPIType
	{
		None = 0,
		OpenGL = 1
	};

	enum class ERenderMode
	{
		Normal = 0,
		Wireframe = 1,
		Point = 2,
	};

	enum class ERendererState
	{
		DEPTH_TEST
	};

	class RenderingAPI
	{
	public:
		/* Destructor */
		virtual ~RenderingAPI() = default;

		/* Methods */
		virtual bool Initialize() = 0;
		virtual void SetClearColor(glm::vec4 color) = 0;
		virtual void SetRenderMode(ERenderMode renderMode) = 0;
		virtual void SetRendererState(ERendererState renderState, bool enabled) = 0;
		virtual void ClearScreen() = 0;
		virtual void ProcessRendereable(int size) = 0;
		virtual void BindTexture(uint32_t textureHandle, uint32_t textureSlot = 0) = 0;
		virtual double GetTime() = 0;

		/* Getters */
		static RenderingAPIType GetRenderingAPIType();

		/* Instantiate */
		static Unique<RenderingAPI> Create(RenderingAPIType renderingAPIType);
	private:
		static RenderingAPIType s_RenderingAPIType;
	};
}
