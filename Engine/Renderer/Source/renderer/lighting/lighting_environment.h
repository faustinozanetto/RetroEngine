#pragma once

#include <glm/glm.hpp>

#include "core/base.h"
#include "renderer/buffers/vao/vertex_array_buffer.h"
#include "renderer/shader/shader.h"
#include "renderer/texture/texture_cubemap.h"

namespace retro::renderer
{
	struct lighting_environment_specification
	{
		shared<texture_cubemap> cubemap;
		uint32_t capture_size;
		uint32_t irradiance_size;
		uint32_t prefilter_size;
		uint32_t brdf_lut_size;

		lighting_environment_specification() = default;

		lighting_environment_specification(const shared<texture_cubemap>& cubemap, uint32_t capture_size,
			uint32_t irradiance_size, uint32_t prefilter_size, uint32_t brdf_lut_size) :
			cubemap(cubemap), capture_size(capture_size),
			irradiance_size(irradiance_size), prefilter_size(prefilter_size), brdf_lut_size(brdf_lut_size)
		{
		}
	};

	class lighting_environment
	{
	public:
		lighting_environment(const lighting_environment_specification& lighting_environment_specification);
		~lighting_environment();

		void render_environment() const;

		uint32_t get_hdri_texture() const { return m_lighting_environment_specification.cubemap->get_object_handle(); }
		uint32_t get_irradiance_texture() const { return irradianceMap; }
		uint32_t get_prefilter_texture() const { return prefilterMap; }
		uint32_t get_brdf_lut_texture() const { return brdfLUTTexture; }

		const lighting_environment_specification& get_lighting_environment_specification() const { return m_lighting_environment_specification; }

		void set_view_projection(const glm::mat4& view, const glm::mat4& projection);

		static shared<lighting_environment> create(const lighting_environment_specification& lighting_environment_specification);

	private:
		void render_environment_cube() const;
		void render_environment_quad() const;

		void setup_capture_frame_buffer();
		void convert_to_cubemap(const glm::mat4& projection, const glm::mat4 view[]);
		void setup_irradiance_map(const glm::mat4& projection, const glm::mat4 view[]);
		void setup_prefilter_map(const glm::mat4& projection, const glm::mat4 view[]);
		void setup_brdf_lut();

	private:
		/* Configuration */
		lighting_environment_specification m_lighting_environment_specification;
		/* Textures */
		uint32_t captureFBO{};
		uint32_t captureRBO{};
		uint32_t envCubemap{};
		uint32_t irradianceMap{};
		uint32_t prefilterMap{};
		uint32_t brdfLUTTexture{};
		/* Mats */
		glm::mat4 m_view{}, m_projection{};
		/* VAOs */
		shared<vertex_array_buffer> m_skybox_vao;
		shared<vertex_array_buffer> m_quad_vao;
		/* Shaders */
		shared<shader> m_skybox_shader;
		shared<shader> m_equirectangular_to_cubemap_shader;
		shared<shader> m_irradiance_convolution_shader;
		shared<shader> m_prefilter_shader;
		shared<shader> m_brdf_lut_shader;
	};
}
