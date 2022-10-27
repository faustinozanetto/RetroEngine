#include "pch.h"

#include "window.h"

#ifdef RETRO_PLATFORM_WINDOWS
#include "platform/open_gl/window/open_gl_window.h"
#endif

namespace Retro::Renderer
{
	const window_specification& window::get_window_specification()
	{
		return m_window_specification;
	}

	bool window::is_vsync_enabled() const
	{
		return m_window_specification.v_sync;
	}

	unique<window> window::create(const window_specification& window_specification)
	{
#ifdef RETRO_PLATFORM_WINDOWS
		return create_unique<open_gl_window>(window_specification);
#else
		logger::error("window::create | Couldnt create window.");
		return nullptr;
#endif
	}
}
