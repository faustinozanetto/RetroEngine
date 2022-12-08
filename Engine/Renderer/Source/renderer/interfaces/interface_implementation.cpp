#include "pch.h"

#include "interface_implementation.h"
#include "platform/open_gl/interfaces/open_gl_interface_implementation.h"
#include "renderer/renderer/renderer.h"

namespace retro::renderer
{
	unique<interface_implementation> interface_implementation::create()
	{
		switch (renderer::get_renderer_api_type())
		{
		case renderer_api_type::none:
		{
			logger::error("interface_implementation::create | Unknown renderer api!.");
			return {};
		}
		case renderer_api_type::open_gl:
		{
			return create_unique<open_gl_interface_implementation>();
		}
		}
		return {};
	}
}