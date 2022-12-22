#include "pch.h"

#include "texture_3d.h"

#include "platform/open_gl/texture/open_gl_texture_3d.h"
#include "renderer/renderer/renderer_context.h"
#include "renderer/renderer/renderer.h"

namespace retro::renderer
{
	shared<texture_3d> texture_3d::create(const texture_specification& texture_specification)
	{
		switch (renderer::get_renderer_api_type())
		{
		case renderer_api_type::none:
		{
			logger::error("texture_3d::create | Unknown renderer api!.");
			return nullptr;
		}
		case renderer_api_type::open_gl:
		{
			return create_shared<open_gl_texture_3d>(texture_specification);
		}
		}
		return {};
	}
}