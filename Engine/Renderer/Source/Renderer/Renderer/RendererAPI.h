#pragma once

#include "Core/Base.h"
#include "glm/glm.hpp"

namespace Retro {
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

		/* Instantiate */
		static Scope<RenderingAPI> Create(RenderingAPIType renderingAPIType);
	};
}