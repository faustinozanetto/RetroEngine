#include "pch.h"

#include "texture_2d.h"

#include "platform/open_gl/texture/open_gl_texture_2d.h"
#include "renderer/renderer/renderer_context.h"
#include "renderer/renderer/renderer.h"

namespace retro::renderer
{
	shared<texture_2d> texture_2d::create(const texture_specification& texture_specification)
	{
		switch (renderer::get_renderer_api_type())
		{
		case renderer_api_type::none:
		{
			logger::error("texture_2d::create | Unknown renderer api!.");
			return nullptr;
		}
		case renderer_api_type::open_gl:
		{
			return create_shared<open_gl_texture_2d>(texture_specification);
		}
		}
		return {};
	}

	shared<texture_2d> texture_2d::create(uint32_t width, uint32_t height, const unsigned char* data)
	{
		switch (renderer::get_renderer_api_type())
		{
		case renderer_api_type::none:
		{
			logger::error("texture_2d::create | Unknown renderer api!.");
			return nullptr;
		}
		case renderer_api_type::open_gl:
		{
			return create_shared<open_gl_texture_2d>(width, height, data);
		}
		}
		return {};
	}

	shared<texture_2d> texture_2d::create(uint32_t width, uint32_t height, uint32_t channels, const unsigned char* data)
	{
		switch (renderer::get_renderer_api_type())
		{
		case renderer_api_type::none:
		{
			logger::error("texture_2d::create | Unknown renderer api!.");
			return nullptr;
		}
		case renderer_api_type::open_gl:
		{
			return create_shared<open_gl_texture_2d>(width, height, channels, data);
		}
		}
		return {};
	}
}