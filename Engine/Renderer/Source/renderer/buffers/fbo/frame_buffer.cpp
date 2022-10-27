#include "pch.h"

#include "frame_buffer.h"

#include "Platform/OpenGL/Buffers/FBO/OpenGLFrameBuffer.h"
#include "renderer/renderer/renderer_context.h"
#include "renderer/renderer/renderer.h"

namespace Retro::Renderer
{
	frame_buffer::~frame_buffer()
	{
	}

	shared<frame_buffer> frame_buffer::create(const frame_buffer_specification &frame_buffer_specification)
	{
		switch (renderer::get_renderer_api_type())
		{
		case renderer_api_type::none:
			{
				logger::error("frame_buffer::create | Unknown renderer api!.");
				return {};
			}
		case renderer_api_type::open_gl:
			{
				return create_shared<open_gl_frame_buffer>(frame_buffer_specification);
			}
		}
		return {};
	}
}
