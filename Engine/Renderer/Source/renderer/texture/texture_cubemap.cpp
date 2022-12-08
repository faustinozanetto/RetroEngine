#include "pch.h"

#include "texture_cubemap.h"

#include "platform/open_gl/texture/open_gl_texture_cubemap.h"
#include "renderer/renderer/renderer_context.h"
#include "renderer/renderer/renderer.h"

namespace retro::renderer
{
	shared<texture_cubemap> texture_cubemap::create(const texture_specification& texture_specification)
	{
		switch (renderer::get_renderer_api_type())
		{
		case renderer_api_type::none:
		{
			logger::error("texture_cubemap::create | Unknown renderer api!.");
			return nullptr;
		}
		case renderer_api_type::open_gl:
		{
			return create_shared<open_gl_texture_cubemap>(texture_specification);
		}
		}
		return {};
	}
}