#include "pch.h"

#include "renderer/shader/shader.h"

#include "renderer/renderer/renderer_context.h"
#include "renderer/renderer/renderer.h"
#include "platform/open_gl/shader/open_gl_shader.h"

namespace retro::renderer
{
	shared<shader> shader::create(const std::string& vertex_path, const std::string& fragment_path)
	{
		switch (renderer::get_renderer_api_type())
		{
		case renderer_api_type::none:
		{
			logger::error("shader::create | Unknown renderer api!.");
			return nullptr;
		}
		case renderer_api_type::open_gl:
		{
			return create_shared<open_gl_shader>(vertex_path, fragment_path);
		}
		}
		return {};
	}

	shared<shader> shader::create(const std::string& vertex_path, const std::string& fragment_path,
		const std::string& geometry_path)
	{
		switch (renderer::get_renderer_api_type())
		{
		case renderer_api_type::none:
			{
				logger::error("shader::create | Unknown renderer api!.");
				return nullptr;
			}
		case renderer_api_type::open_gl:
			{
				return create_shared<open_gl_shader>(vertex_path, fragment_path, geometry_path);
			}
		}
		return {};
	}
}
