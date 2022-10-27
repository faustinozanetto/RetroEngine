#include "pch.h"

#include "renderer/renderer/renderer_api.h"
#include "platform/open_gl/renderer/open_gl_renderer_api.h"

namespace retro::renderer
{
	renderer_api_type renderer_api::s_renderer_api_type = renderer_api_type::open_gl;

	renderer_api_type renderer_api::get_renderer_api_type()
	{
		return s_renderer_api_type;
	}

	unique<renderer_api> renderer_api::create(renderer_api_type renderer_api_type)
	{
		switch (renderer_api_type)
		{
		case renderer_api_type::none:
		{
			logger::error("renderer_api::create | Unknown renderer api!.");
			return nullptr;
		}
		case renderer_api_type::open_gl:
		{
			return create_unique<open_gl_renderer_api>();
		}
		}
		return {};
	}
}
