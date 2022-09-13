#pragma once

#include "Core/Base.h"

namespace Retro {

	class RendererContext
	{
	public:
		/* Destructor */
		virtual ~RendererContext() = default;

		/* Methods */
		virtual void Initialize() = 0;
		virtual void SwapBuffers() = 0;
		virtual void PollInput() = 0;

		/* Instantiate */
		static Scope<RendererContext> Create(void* windowHandle);
	};

}