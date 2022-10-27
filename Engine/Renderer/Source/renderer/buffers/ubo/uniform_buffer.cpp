#include "pch.h"

#include "uniform_buffer.h"

#include "platform/open_gl/buffers/ubo/open_gl_uniform_buffer.h"
#include "renderer/renderer/renderer_context.h"
#include "renderer/renderer/renderer.h"

namespace retro::renderer
{
	uniform_buffer::~uniform_buffer()
	{
	}

	shared<uniform_buffer> uniform_buffer::create(uint32_t size, uint32_t binding)
	{
		switch (renderer::get_renderer_api_type())
		{
		case renderer_api_type::none:
		{
			logger::error("uniform_buffer::create | Unknown renderer api!.");
			return {};
		}
		case renderer_api_type::open_gl:
		{
			return create_shared<open_gl_uniform_buffer>(size, binding);
		}
		}
		return {};
	}
}
