#include "pch.h"

#include "texture.h"

#include "platform/open_gl/texture/open_gl_texture.h"
#include "renderer/renderer/renderer_context.h"
#include "renderer/renderer/renderer.h"

namespace retro::renderer
{
	std::string texture::get_texture_filtering_to_string(texture_filtering texture_filtering)
	{
		switch (texture_filtering)
		{
		case texture_filtering::none: return "None";
		case texture_filtering::linear: return "Linear";
		case texture_filtering::nearest: return "Nearest";
		}
		return "Unknown";
	}

	std::string texture::get_texture_wrapping_to_string(texture_wrapping texture_wrapping)
	{
		switch (texture_wrapping)
		{
		case texture_wrapping::none: return "None";
		case texture_wrapping::repeat: return "Repeat";
		case texture_wrapping::clamp_border: return "ClampBorder";
		case texture_wrapping::clamp_edge: return "ClampEdge";
		case texture_wrapping::mirror_repeat: return "MirrorRepeat";
		}
		return "Unknown";
	}

	shared<texture> texture::create(const texture_specification& texture_specification)
	{
		switch (renderer::get_renderer_api_type())
		{
		case renderer_api_type::none:
		{
			logger::error("texture::create | Unknown renderer api!.");
			return nullptr;
		}
		case renderer_api_type::open_gl:
		{
			return create_shared<open_gl_texture>(texture_specification);
		}
		}
		return {};
	}

	shared<texture> texture::create(uint32_t width, uint32_t height, const unsigned char* data)
	{
		switch (renderer::get_renderer_api_type())
		{
		case renderer_api_type::none:
		{
			logger::error("texture::create | Unknown renderer api!.");
			return nullptr;
		}
		case renderer_api_type::open_gl:
		{
			return create_shared<open_gl_texture>(width, height,data);
		}
		}
		return {};
	}
}
