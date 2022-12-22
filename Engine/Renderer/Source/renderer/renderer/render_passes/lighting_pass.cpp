#include "pch.h"

#include "lighting_pass.h"

#include "core/assets/assets_manager.h"
#include "renderer/renderer/renderer_api.h"
#include "renderer/renderer/renderer.h"

#include "glad/glad.h"
#include "renderer/renderer/scene_renderer.h"

namespace retro::renderer
{
	lighting_pass::lighting_pass()
	{
		load_shaders();
		generate_frame_buffer();
	}

	lighting_pass::~lighting_pass()
	{
	}

	void lighting_pass::begin_pass()
	{
		m_lighthing_frame_buffer->bind();
		renderer::set_renderer_state(renderer_state::depth_test, false);
		m_lighting_shader->bind();

		const auto& shadow_map_pass = scene_renderer::get_shadow_pass();

		m_lighting_shader->set_int("u_blocker_search_samples",
			shadow_map_pass->get_blocker_samples());
		m_lighting_shader->set_int("u_pcf_samples",
			shadow_map_pass->get_pcf_samples());
		m_lighting_shader->set_float("u_light_radius_uv",
			shadow_map_pass->get_light_radius() / (shadow_map_pass->get_frustum_size() * 2.0f));
		m_lighting_shader->set_float("u_light_near",
			shadow_map_pass->get_frustum_planes().x);
		m_lighting_shader->set_float("u_light_far",
			shadow_map_pass->get_frustum_planes().y);

		m_lighting_shader->set_float("pointLight.radius",
			scene_renderer::get_data().m_pointLight.radius);
		m_lighting_shader->set_float("pointLight.intensity",
			scene_renderer::get_data().m_pointLight.intensity);
		m_lighting_shader->set_vec_float4("pointLight.color",
			scene_renderer::get_data().m_pointLight.color);
		m_lighting_shader->set_vec_float4("pointLight.position",
			scene_renderer::get_data().m_pointLight.position);

		m_lighting_shader->set_float("directionalLight.intensity",
			scene_renderer::get_data().m_directional_light.intensity);
		m_lighting_shader->set_vec_float4("directionalLight.color",
			scene_renderer::get_data().m_directional_light.color);
		m_lighting_shader->set_vec_float4("directionalLight.direction",
			scene_renderer::get_data().m_directional_light.direction);
		m_lighting_shader->set_int("u_ssao_enabled", scene_renderer::get_ssao_pass()->render_pass_enabled());

		glBindSampler(9, scene_renderer::get_shadow_pass()->get_pcf_sampler());
		bind_inputs();
		renderer::submit_command({ m_lighting_shader, scene_renderer::get_screen_vao(), nullptr });

		m_lighting_shader->un_bind();
		m_lighthing_frame_buffer->un_bind();
	}

	void lighting_pass::resize(uint32_t width, uint32_t height)
	{
		m_lighthing_frame_buffer->resize(width, height);
	}

	shared<frame_buffer>& lighting_pass::get_pass_output()
	{
		return m_lighthing_frame_buffer;
	}

	void lighting_pass::initialize_inputs()
	{
		const auto& geometry_pass = scene_renderer::get_geometry_pass();
		set_pass_input(0, geometry_pass->get_pass_output()->get_attachment_id(0)); // Position
		set_pass_input(1, geometry_pass->get_pass_output()->get_attachment_id(1)); // Albedo
		set_pass_input(2, geometry_pass->get_pass_output()->get_attachment_id(2)); // Normal
		set_pass_input(3, geometry_pass->get_pass_output()->get_attachment_id(3)); // Rough-Metal-AO
		set_pass_input(4, geometry_pass->get_pass_output()->get_attachment_id(4)); // ViewPos
		const auto& lighting_environment = scene_renderer::get_lighting_environment();
		set_pass_input(5, lighting_environment->get_irradiance_texture()); // Irradiance
		set_pass_input(6, lighting_environment->get_prefilter_texture()); // Prefilter
		set_pass_input(7, lighting_environment->get_brdf_lut_texture()); // BRDF
		const auto& shadow_map_pass = scene_renderer::get_shadow_pass();
		set_pass_input(8, shadow_map_pass->get_pass_output()->get_depth_attachment_id()); // ShadowMap
		set_pass_input(9, shadow_map_pass->get_pass_output()->get_depth_attachment_id()); // ShadowMap
		set_pass_input(10, shadow_map_pass->get_random_angles_tex()); // Random Angles
		set_pass_input(11, scene_renderer::get_ssao_pass()->get_ssao_result()); // SSAO
	}

	void lighting_pass::bind_inputs()
	{
		for (const auto& [index, input_handle] : m_pass_inputs)
		{
			renderer::bind_texture(input_handle, index);
		}
	}

	void lighting_pass::generate_frame_buffer()
	{
		texture_specification lighting_tex_spec = {
				glm::uvec2(1920, 1080),
				texture_filtering::linear,
				texture_wrapping::clamp_edge,
				GL_RGBA, GL_RGB16F };
		m_lighthing_frame_buffer =
			frame_buffer::create({ 1920, 1080,
				false, {{"lighting", lighting_tex_spec}} });
	}

	void lighting_pass::load_shaders()
	{
		m_lighting_shader = assets_manager::get().create_shader({
				"Assets/Shaders/Lighting/Lighting.vert",
				"Assets/Shaders/Lighting/Lighting.frag" });
	}
}