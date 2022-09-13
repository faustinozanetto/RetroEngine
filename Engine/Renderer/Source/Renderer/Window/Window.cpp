#include "pch.h"

#include "Window.h"

#ifdef RETRO_PLATFORM_WINDOWS
#include "Platform/OpenGL/Window/OpenGLWindow.h"
#endif

namespace Retro::Renderer {
	const FWindowSpecification& Window::GetWindowSpecification()
	{
		return m_WindowSpecification;
	}

	bool Window::IsVSyncEnabled() const
	{
		return m_WindowSpecification.vSync;
	}

	Scope<Window> Window::Create(const FWindowSpecification& specification)
	{
#ifdef RETRO_PLATFORM_WINDOWS
		return CreateScope<OpenGLWindow>(specification);
#else
		Logger::Error("Window::Create | Couldnt create window.");
		return nullptr;
#endif
	}
}


