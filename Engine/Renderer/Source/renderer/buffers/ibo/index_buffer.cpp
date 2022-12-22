#include "pch.h"

#include "index_buffer.h"

#include "platform/open_gl/buffers/ibo/open_gl_index_buffer.h"
#include "renderer/renderer/renderer_context.h"
#include "renderer/renderer/renderer.h"

namespace retro::renderer
{
	index_buffer::~index_buffer()
	{
	}

	shared<index_buffer> index_buffer::create(uint32_t* data, uint32_t size)
	{
		switch (renderer::get_renderer_api_type())
		{
		case renderer_api_type::none:
		{
			logger::error("index_buffer::create | Unknown renderer api!.");
			return {};
		}
		case renderer_api_type::open_gl:
		{
			return create_shared<open_gl_index_buffer>(data, size);
		}
		}
		return {};
	}
}