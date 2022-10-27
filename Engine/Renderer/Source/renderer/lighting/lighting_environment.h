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

		uint32_t get_irradiance_texture() const { return irradianceMap; }
		uint32_t get_prefilter_texture() const { return prefilterMap; }
		uint32_t get_brdf_lut_texture() const { return brdfLUTTexture; }

		static shared<lighting_environment> create(const shared<texture_cubemap> texture_cubemap);

	private:
		void render_environment_cube();
		void render_environment_quad();

	private:
		uint32_t envCubemap;
		uint32_t irradianceMap;
		uint32_t prefilterMap;
		uint32_t brdfLUTTexture;
		glm::mat4 m_view, m_projection;
		shared<texture_cubemap> m_texture_cubemap;
		shared<vertex_array_buffer> m_skybox_vao;
		shared<vertex_array_buffer> m_quad_vao;
		shared<shader> m_skybox_shader;
		shared<shader> m_equirectangular_to_cubemap_shader;
		shared<shader> m_irradiance_convolution_shader;
		shared<shader> m_prefilter_shader;
		shared<shader> m_brdf_lut_shader;
	};
}
