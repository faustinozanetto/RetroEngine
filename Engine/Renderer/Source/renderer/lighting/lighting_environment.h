#pragma once

#include <glm/glm.hpp>

#include "core/base.h"
#include "renderer/buffers/vao/vertex_array_buffer.h"
#include "renderer/shader/shader.h"
#include "renderer/texture/texture_cubemap.h"

namespace retro::renderer
{
	class lighting_environment
	{
	public:
		lighting_environment(const shared<texture_cubemap> texture_cubemap);
		~lighting_environment();

		void set_view_projection(const glm::mat4& view, const glm::mat4& projection) { m_view = view; m_projection = projection; }
		void render_environment();

		static shared<lighting_environment> create(const shared<texture_cubemap> texture_cubemap);

	private:
		unsigned int envCubemap;
		glm::mat4 m_view, m_projection;
		shared<texture_cubemap> m_texture_cubemap;
		shared<vertex_array_buffer> m_skybox_vao;
		shared<shader> m_skybox_shader;
	};
}
