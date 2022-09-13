#pragma once

#include "Core/Base.h"
#include "glm/glm.hpp"

namespace Retro::Renderer {
	enum class RenderingAPIType {
		None = 0,
		OpenGL = 1
	};

	class RenderingAPI {
	public:
		/* Destructor */
		virtual ~RenderingAPI() = default;

		/* Methods */
		virtual bool Initialize() = 0;
		virtual void SetClearColor(glm::vec4 color) = 0;
		virtual void ClearScreen() = 0;

		/* Getters */
		static RenderingAPIType GetRenderingAPIType();

		/* Instantiate */
		static Scope<RenderingAPI> Create(RenderingAPIType renderingAPIType);
	private:
		static RenderingAPIType s_RenderingAPIType;
	};
}