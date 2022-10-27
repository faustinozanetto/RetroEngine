#include "pch.h"

#include "vertex_array_buffer.h"

#include "platform/open_gl/buffers/vao/open_gl_vertex_array_buffer.h"
#include "renderer/renderer/renderer_context.h"
#include "renderer/renderer/renderer.h"

namespace retro::renderer
{
	vertex_array_buffer::vertex_array_buffer()
	{
	}

	vertex_array_buffer::~vertex_array_buffer()
	{
	}

	shared<vertex_array_buffer> vertex_array_buffer::create()
	{
		switch (renderer::get_renderer_api_type())
		{
		case renderer_api_type::none:
		{
			logger::error("vertex_array_buffer::create | Unknown renderer api!.");
			return {};
		}
		case renderer_api_type::open_gl:
		{
			return create_shared<open_gl_vertex_array_buffer>();
		}
		}
		return {};
	}
}
