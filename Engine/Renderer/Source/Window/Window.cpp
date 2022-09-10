﻿#include "pch.h"
#include "Window.h"

#ifdef RETRO_PLATFORM_WINDOWS
#include "Platform/OpenGL/Window/OpenGLWindow.h"
#endif

namespace Retro::Renderer {
	Core::Scope<Window> Window::Create(const FWindowSpecification& specification)
	{
#ifdef RETRO_PLATFORM_WINDOWS
		return Core::CreateScope<OpenGLWindow>(specification);
#else
		Core::Logger::Error("Window::Create | Couldnt create window.");
		return nullptr;
#endif
	}
}


