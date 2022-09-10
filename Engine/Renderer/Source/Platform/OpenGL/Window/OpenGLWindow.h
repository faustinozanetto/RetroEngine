#pragma once

#include "Window/Window.h"

namespace Retro::Renderer {
	class OpenGLWindow : public Window {
	public:
		/* Constructor & Destructor */
		OpenGLWindow(const FWindowSpecification& specification);
		~OpenGLWindow() override;
	};

}