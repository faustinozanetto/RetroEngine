#include "pch.h"

#include "shadow_map_pass.h"

#include <glm/glm.hpp>
#include <glm/gtc/type_ptr.hpp>

#include "imgui.h"
#include "glad/glad.h"

#include "core/application/retro_application.h"
#include "core/scene/components.h"
#include "renderer/renderer/renderer.h"

#define SHADOW_UBO_LOCATION 4

namespace retro::renderer
{
	shadow_map_pass::shadow_map_pass()
	{
		m_shadow_map_res = glm::vec2(4096, 4096);
		m_dir_shadow_frustum_planes = glm::vec2(120, 250);
		m_dir_shadow_frustum_size = 20.0f;
		m_scene_size = glm::uvec2(10);

		load_shaders();
		update_shadow_matrices({ -0.77f, 0.55f, 0.25f });
		generate_shadow_ubo();
		generate_shadow_map();
		generate_shadow_fbo();
		generate_pcf_sampler();
		generate_random_angles_tex();
	}

	shadow_map_pass::~shadow_map_pass()
	{
	}

	void shadow_map_pass::begin_pass()
	{
		ImGui::Begin("PCF Shadows");
		ImGui::SliderFloat("Light radius", &m_light_radius, 0.0, 1.0, "%.2f");
		ImGui::SliderFloat2("Scene Size", glm::value_ptr(m_scene_size), 0, 50);
		ImGui::SliderInt("PCF Samples", &m_pcf_samples, 0, 128);
		ImGui::SliderInt("Blocker Samples", &m_blocker_samples, 0, 128);

		ImGui::End();

		// Prepare for shadow pass.
		m_shadow_fbo->bind();
		//renderer::set_renderer_state(renderer_state::depth_test, true);
		//renderer::set_clear_color({ 0.0f, 0.0f, 0.0f, 1.0f });
		glClear(GL_DEPTH_BUFFER_BIT);

		glCullFace(GL_FRONT);

		m_shadow_shader->bind();
		//renderer::clear_screen();

		const auto view = retro_application::get_application().get_scene_manager()->get_active_scene()->get_actor_registry().group<model_renderer_component>(
			entt::get<transform_component>);
		for (auto&& [actor, model_renderer, transform] : view.each())
		{
			m_shadow_shader->set_mat4("u_model", transform.get_transform_matrix());
			for (const auto& renderable : model_renderer.model->get_model_renderables())
			{
				renderer::submit_command({
						m_shadow_shader,
						renderable->get_vertex_array_buffer(),
						nullptr,
					});
			}
		}

		m_shadow_shader->un_bind();
		m_shadow_fbo->un_bind();
		glCullFace(GL_BACK);
	}

	shared<frame_buffer>& shadow_map_pass::get_pass_output()
	{
		return m_shadow_fbo;
	}

	void shadow_map_pass::update_shadow_matrices(const glm::vec3& light_dir)
	{
		const glm::vec3 max_extents = glm::vec3(m_scene_size.x);
		const glm::vec3 min_extents = glm::vec3(-m_scene_size.y);
		const glm::vec3 scene_center = (max_extents + min_extents) * 0.5f;

		const glm::mat4 light_view_matrix = glm::lookAt(scene_center - light_dir * -min_extents.z, scene_center,
			glm::vec3(0.0f, 1.0f, 0.0f));
		const glm::mat4 light_ortho_matrix = glm::ortho(min_extents.x, max_extents.x, min_extents.y, max_extents.y, 0.0f,
			max_extents.z - min_extents.z);

		m_dir_shadow_frustum_size = max_extents.x - min_extents.x;

		m_dir_light_view = light_view_matrix;
		m_dir_light_view_projection = light_ortho_matrix * light_view_matrix;
		m_dir_shadow_frustum_planes = glm::vec2(min_extents.z, max_extents.z);
	}

	void shadow_map_pass::generate_shadow_ubo()
	{
		m_shadow_ubo = uniform_buffer::create(sizeof(shadow_data), SHADOW_UBO_LOCATION);
		update_shadow_ubo();
	}

	void shadow_map_pass::update_shadow_ubo()
	{
		m_shadow_data.light_view_projection = m_dir_light_view_projection;
		m_shadow_data.light_view = m_dir_light_view;
		m_shadow_ubo->set_data(&m_shadow_data, sizeof(shadow_data));
	}

	void shadow_map_pass::generate_shadow_map()
	{
		/*
		m_shadow_map = retro_application::get_application().get_assets_manager()->create_texture(
			{
				m_shadow_map_res, texture_filtering::nearest, texture_wrapping::clamp_border, GL_DEPTH_COMPONENT, GL_DEPTH_COMPONENT });
				*/
		GLfloat border[] = { 1.0, 0.0, 0.0, 0.0 };

		glCreateTextures(GL_TEXTURE_2D, 1, &m_shadow_map);
		glTextureStorage2D(m_shadow_map, 1, GL_DEPTH_COMPONENT32F, m_shadow_map_res.x,
			m_shadow_map_res.y);

		glTextureParameteri(m_shadow_map, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
		glTextureParameteri(m_shadow_map, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
		glTextureParameteri(m_shadow_map, GL_TEXTURE_WRAP_S,
			GL_CLAMP_TO_BORDER);
		glTextureParameteri(m_shadow_map, GL_TEXTURE_WRAP_T,
			GL_CLAMP_TO_BORDER);
		glTextureParameterfv(m_shadow_map, GL_TEXTURE_BORDER_COLOR, border);
	}

	void shadow_map_pass::generate_shadow_fbo()
	{
		texture_specification shadow_tex_spec = {
			glm::uvec2(4096, 4096),
			texture_filtering::nearest,
			texture_wrapping::clamp_border,
			GL_DEPTH_COMPONENT32F,
			GL_RGB16F };
		m_shadow_fbo = frame_buffer::create({ 4096, 4096, {{"shadow", {shadow_tex_spec}}} });
	}

	void shadow_map_pass::generate_random_angles_tex()
	{
		int size = 128;
		int buffer_size = size * size * size;
		auto data = std::make_unique<glm::vec2[]>(buffer_size); // glm::vec2[buffer_size];

		for (int z = 0; z < size; ++z)
		{
			for (int y = 0; y < size; ++y)
			{
				for (int x = 0; x < size; ++x)
				{
					int index = x + y * size + z * size * size;
					static std::uniform_real_distribution<double> distribution(0.0, 1.0);
					static std::mt19937 generator;
					float random_angle = glm::two_pi<double>() * distribution(generator);
					// Random angles in range [0, 2PI);
					data[index] = glm::vec2(glm::cos(random_angle), glm::sin(random_angle));
					// Map sine and cosine values to [0, 1] range
				}
			}
		}

		glCreateTextures(GL_TEXTURE_3D, 1, &m_random_angles_tex3d_id);
		glTextureStorage3D(m_random_angles_tex3d_id, 1, GL_RG32F, size, size, size);
		glTextureSubImage3D(m_random_angles_tex3d_id, 0, 0, 0, 0, size, size, size, GL_RG, GL_FLOAT, data.get());

		glTextureParameteri(m_random_angles_tex3d_id, GL_TEXTURE_WRAP_S, GL_REPEAT);
		glTextureParameteri(m_random_angles_tex3d_id, GL_TEXTURE_WRAP_T, GL_REPEAT);
		glTextureParameteri(m_random_angles_tex3d_id, GL_TEXTURE_WRAP_R, GL_REPEAT);
		glTextureParameteri(m_random_angles_tex3d_id, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
		glTextureParameteri(m_random_angles_tex3d_id, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
	}

	void shadow_map_pass::generate_pcf_sampler()
	{
		glCreateSamplers(1, &m_pcf_sampler);
		glSamplerParameteri(m_pcf_sampler, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
		glSamplerParameteri(m_pcf_sampler, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
		glSamplerParameteri(m_pcf_sampler, GL_TEXTURE_WRAP_S,
			GL_CLAMP_TO_BORDER);
		glSamplerParameteri(m_pcf_sampler, GL_TEXTURE_WRAP_T,
			GL_CLAMP_TO_BORDER);
		float color[4] = { 1, 0, 0, 0 };
		glSamplerParameterfv(m_pcf_sampler, GL_TEXTURE_BORDER_COLOR, color);
		glSamplerParameteri(m_pcf_sampler, GL_TEXTURE_COMPARE_MODE,
			GL_COMPARE_REF_TO_TEXTURE);
		glSamplerParameteri(m_pcf_sampler, GL_TEXTURE_COMPARE_FUNC, GL_LEQUAL);
	}

	void shadow_map_pass::load_shaders()
	{
		m_shadow_shader = retro_application::get_application().get_assets_manager()->create_shader(
			{
					"Assets/Shaders/Shadows/GeneratePCSS.vert", "Assets/Shaders/Shadows/GeneratePCSS.frag" });
	}
}