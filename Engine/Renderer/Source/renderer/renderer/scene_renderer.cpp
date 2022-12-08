#include "pch.h"

#include "scene_renderer.h"

#include "renderer.h"
#include "core/scene/components.h"
#include "core/scene/actor.h"
#include "glad/glad.h"
#include "renderer/lighting/point_light.h"

#include <glm/glm.hpp>
#include <glm/gtc/type_ptr.hpp>

#include "imgui.h"
#include "core/application/retro_application.h"
#include "renderer/lighting/directional_light.h"

namespace retro::renderer
{
	static scene_renderer_data s_scene_renderer_data;

	void scene_renderer::initialize(const shared<camera>& camera)
	{
		logger::line();
		logger::info("Initializing scene_renderer");
		load_shaders();
		generate_frame_buffers();
		create_screen_vao();
		create_camera(camera);
		setup_lights();
		setup_environment();

		s_scene_renderer_data.shadow_map_pass = create_shared<shadow_map_pass>();
		s_scene_renderer_data.geometry_pass = create_shared<geometry_pass>();
	}

	void scene_renderer::begin_render()
	{
		// Update camera UBO.
		s_scene_renderer_data.m_camera->update();
		s_scene_renderer_data.m_camera_data.u_ViewMatrix = s_scene_renderer_data.m_camera->get_view_matrix();
		s_scene_renderer_data.m_camera_data.u_ProjectionMatrix = s_scene_renderer_data.m_camera->get_projection_matrix();
		s_scene_renderer_data.m_camera_data.u_Position = s_scene_renderer_data.m_camera->get_position();
		s_scene_renderer_data.m_camera_data.u_ViewProjectionMatrix = s_scene_renderer_data.m_camera->get_view_projection();
		s_scene_renderer_data.m_camera_ubo->set_data(&s_scene_renderer_data.m_camera_data, sizeof(camera_data));

		// Update lights UBO
		const auto lights = s_scene_renderer_data.m_scene->get_actor_registry().view<light_renderer_component, name_component, transform_component>();
		for (auto&& [actor, light, name, transform] : lights.each())
		{
			if (light.type == light_type::point)
			{
				auto point_light = dynamic_cast<retro::renderer::point_light*>(light.light.get());
				s_scene_renderer_data.m_pointLight.position = glm::vec4(transform.position, 0);
				s_scene_renderer_data.m_pointLight.color = glm::vec4(point_light->get_color(), 0);
				s_scene_renderer_data.m_pointLight.intensity = point_light->get_intensity();
				s_scene_renderer_data.m_pointLight.radius = point_light->get_radius();
				point_light = nullptr;
			}
			else if (light.type == light_type::directional)
			{
				auto directional_light = dynamic_cast<retro::renderer::directional_light*>(light.light.get());
				s_scene_renderer_data.m_directional_light.direction = glm::vec4(directional_light->get_direction(), 0);
				s_scene_renderer_data.m_directional_light.color = glm::vec4(directional_light->get_color(), 0);
				s_scene_renderer_data.m_directional_light.intensity = directional_light->get_intensity();
				directional_light = nullptr;
			}
		}

		s_scene_renderer_data.m_lights_ubo->set_data(&s_scene_renderer_data.m_pointLight, sizeof(point_light_data));
		s_scene_renderer_data.m_lights_ubo->set_data(&s_scene_renderer_data.m_directional_light,
			sizeof(directional_light_data), sizeof(point_light_data));

		s_scene_renderer_data.m_lighting_environment->set_view_projection(
			s_scene_renderer_data.m_camera_data.u_ViewMatrix, s_scene_renderer_data.m_camera_data.u_ProjectionMatrix);

		const auto view = s_scene_renderer_data.m_scene->get_actor_registry().group<model_renderer_component>(
			entt::get<name_component, transform_component>);

		/* ==================== SHADOW PASS ==================== */
		s_scene_renderer_data.shadow_map_pass->begin_pass();

		/* ==================== GEOMETRY PASS ==================== */
		s_scene_renderer_data.geometry_pass->begin_pass();

		/*==================== LIGHTING PASS ==================== */
		s_scene_renderer_data.m_final_frame_buffer->bind();
		renderer::set_renderer_state(renderer_state::depth_test, false);
		s_scene_renderer_data.m_lighting_shader->bind();

		s_scene_renderer_data.m_lighting_shader->set_int("u_blocker_search_samples",
			s_scene_renderer_data.shadow_map_pass->get_blocker_samples());
		s_scene_renderer_data.m_lighting_shader->set_int("u_pcf_samples",
			s_scene_renderer_data.shadow_map_pass->get_pcf_samples());
		s_scene_renderer_data.m_lighting_shader->set_float("u_light_radius_uv",
			s_scene_renderer_data.shadow_map_pass->get_light_radius() / (s_scene_renderer_data.shadow_map_pass->get_frustum_size() * 2.0f));
		s_scene_renderer_data.m_lighting_shader->set_float("u_light_near",
			s_scene_renderer_data.shadow_map_pass->get_frustum_planes().x);
		s_scene_renderer_data.m_lighting_shader->set_float("u_light_far",
			s_scene_renderer_data.shadow_map_pass->get_frustum_planes().y);

		s_scene_renderer_data.m_lighting_shader->set_float("pointLight.radius",
			s_scene_renderer_data.m_pointLight.radius);
		s_scene_renderer_data.m_lighting_shader->set_float("pointLight.intensity",
			s_scene_renderer_data.m_pointLight.intensity);
		s_scene_renderer_data.m_lighting_shader->set_vec_float4("pointLight.color",
			s_scene_renderer_data.m_pointLight.color);
		s_scene_renderer_data.m_lighting_shader->set_vec_float4("pointLight.position",
			s_scene_renderer_data.m_pointLight.position);

		s_scene_renderer_data.m_lighting_shader->set_float("directionalLight.intensity",
			s_scene_renderer_data.m_directional_light.intensity);
		s_scene_renderer_data.m_lighting_shader->set_vec_float4("directionalLight.color",
			s_scene_renderer_data.m_directional_light.color);
		s_scene_renderer_data.m_lighting_shader->set_vec_float4("directionalLight.direction",
			s_scene_renderer_data.m_directional_light.direction);

		renderer::bind_texture(s_scene_renderer_data.geometry_pass->get_pass_output()->get_attachment_id(0), 0); // position
		renderer::bind_texture(s_scene_renderer_data.geometry_pass->get_pass_output()->get_attachment_id(1), 1); // albedo
		renderer::bind_texture(s_scene_renderer_data.geometry_pass->get_pass_output()->get_attachment_id(2), 2); // normal
		renderer::bind_texture(s_scene_renderer_data.geometry_pass->get_pass_output()->get_attachment_id(3), 3);
		// rough - metal - ao
		renderer::bind_texture(s_scene_renderer_data.geometry_pass->get_pass_output()->get_attachment_id(4), 4); // view position
		renderer::bind_texture(s_scene_renderer_data.m_lighting_environment->get_irradiance_texture(), 5);
		renderer::bind_texture(s_scene_renderer_data.m_lighting_environment->get_prefilter_texture(), 6);
		renderer::bind_texture(s_scene_renderer_data.m_lighting_environment->get_brdf_lut_texture(), 7);
		glBindSampler(9, s_scene_renderer_data.shadow_map_pass->get_pcf_sampler());
		renderer::bind_texture(s_scene_renderer_data.shadow_map_pass->get_pass_output()->get_depth_attachment_id(), 8);
		renderer::bind_texture(s_scene_renderer_data.shadow_map_pass->get_pass_output()->get_depth_attachment_id(), 9);
		renderer::bind_texture(s_scene_renderer_data.shadow_map_pass->get_random_angles_tex(), 10);
		renderer::submit_command({ s_scene_renderer_data.m_lighting_shader, s_scene_renderer_data.m_screen_vao, nullptr });
		s_scene_renderer_data.m_lighting_shader->un_bind();

		/*==================== FINAL PASS ====================*/
		glBindFramebuffer(GL_READ_FRAMEBUFFER, s_scene_renderer_data.geometry_pass->get_pass_output()->get_object_handle());
		glBindFramebuffer(GL_DRAW_FRAMEBUFFER, s_scene_renderer_data.m_final_frame_buffer->get_object_handle());
		glBlitFramebuffer(0, 0, static_cast<int>(s_scene_renderer_data.m_final_frame_buffer->get_width()),
			static_cast<int>(s_scene_renderer_data.m_final_frame_buffer->get_height()), 0, 0,
			static_cast<int>(s_scene_renderer_data.m_final_frame_buffer->get_width()),
			static_cast<int>(s_scene_renderer_data.m_final_frame_buffer->get_height()),
			GL_DEPTH_BUFFER_BIT, GL_NEAREST);
		glEnable(GL_DEPTH_TEST);
		glDepthFunc(GL_LEQUAL);
		s_scene_renderer_data.m_lighting_environment->render_environment();
		glDepthFunc(GL_LESS);
		s_scene_renderer_data.m_final_frame_buffer->un_bind();

		if (s_scene_renderer_data.fxaa_enabled)
		{
			s_scene_renderer_data.m_fxaa_frame_buffer->bind();
			renderer::clear_screen();
			s_scene_renderer_data.m_fxaa_shader->bind();
			s_scene_renderer_data.m_fxaa_shader->set_vec_float2("RCPFrame",
				{ float(1.0 / float(s_scene_renderer_data.m_fxaa_frame_buffer->get_width())),
				 float(1.0 / float(s_scene_renderer_data.m_fxaa_frame_buffer->get_height())) });
			renderer::bind_texture(s_scene_renderer_data.m_final_frame_buffer->get_attachment_id(0), 0);
			renderer::submit_command({ s_scene_renderer_data.m_fxaa_shader, s_scene_renderer_data.m_screen_vao, nullptr });
			s_scene_renderer_data.m_fxaa_shader->un_bind();
			s_scene_renderer_data.m_fxaa_frame_buffer->un_bind();
		}
	}

	void scene_renderer::end_render()
	{
	}

	void scene_renderer::set_scene(const shared<scene>& scene)
	{
		s_scene_renderer_data.m_scene = scene;
	}

	shared<frame_buffer>& scene_renderer::get_geometry_frame_buffer()
	{
		return s_scene_renderer_data.geometry_pass->get_pass_output();
	}

	shared<frame_buffer>& scene_renderer::get_depth_frame_buffer()
	{
		return s_scene_renderer_data.shadow_map_pass->get_pass_output();
	}

	shared<frame_buffer>& scene_renderer::get_final_frame_buffer()
	{
		return s_scene_renderer_data.m_final_frame_buffer;
	}

	shared<frame_buffer>& scene_renderer::get_fxaa_frame_buffer()
	{
		return s_scene_renderer_data.m_fxaa_frame_buffer;
	}

	shared<lighting_environment>& scene_renderer::get_lighting_environment()
	{
		return s_scene_renderer_data.m_lighting_environment;
	}

	shared<camera>& scene_renderer::get_camera()
	{
		return s_scene_renderer_data.m_camera;
	}

	scene_renderer_data& scene_renderer::get_data()
	{
		return s_scene_renderer_data;
	}

	uint32_t scene_renderer::get_final_texture()
	{
		return s_scene_renderer_data.m_final_frame_buffer->get_attachment_id(0);
	}

	GLuint scene_renderer::generate_random_angles_texture_3d(uint32_t size)
	{
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

		GLuint tex_id = 0;
		glCreateTextures(GL_TEXTURE_3D, 1, &tex_id);
		glTextureStorage3D(tex_id, 1, GL_RG32F, size, size, size);
		glTextureSubImage3D(tex_id, 0, 0, 0, 0, size, size, size, GL_RG, GL_FLOAT, data.get());

		glTextureParameteri(tex_id, GL_TEXTURE_WRAP_S, GL_REPEAT);
		glTextureParameteri(tex_id, GL_TEXTURE_WRAP_T, GL_REPEAT);
		glTextureParameteri(tex_id, GL_TEXTURE_WRAP_R, GL_REPEAT);
		glTextureParameteri(tex_id, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
		glTextureParameteri(tex_id, GL_TEXTURE_MIN_FILTER, GL_NEAREST);

		return tex_id;
	}

	void scene_renderer::load_shaders()
	{
		s_scene_renderer_data.m_screen_shader = retro_application::get_application().get_assets_manager()->create_shader({ "Assets/Shaders/Screen/Screen.vert",
																													 "Assets/Shaders/Screen/Screen.frag" });
		s_scene_renderer_data.m_lighting_shader = retro_application::get_application().get_assets_manager()->create_shader({
				"Assets/Shaders/Lighting/Lighting.vert",
				"Assets/Shaders/Lighting/Lighting.frag" });
		s_scene_renderer_data.m_shadow_shader = retro_application::get_application().get_assets_manager()->create_shader({
				"Assets/Shaders/Shadows/Shadows.vert", "Assets/Shaders/Shadows/Shadows.frag" });

		s_scene_renderer_data.m_fxaa_shader = retro_application::get_application().get_assets_manager()->create_shader({ "Assets/Shaders/Screen/Screen.vert", "Assets/Shaders/FXAA/FXAA.frag" });
	}

	void scene_renderer::generate_frame_buffers()
	{
		texture_specification final_tex_spec = {
				glm::uvec2(1920, 1080),
				texture_filtering::linear,
				texture_wrapping::clamp_edge,
				GL_RGBA, GL_RGB16F };
		s_scene_renderer_data.m_final_frame_buffer =
			frame_buffer::create({ 1920, 1080, {{"final", final_tex_spec}} });

		texture_specification fxaa_tex_spec = {
				glm::uvec2(1920, 1080),
				texture_filtering::linear,
				texture_wrapping::clamp_edge,
				GL_RGBA, GL_RGB16F };
		s_scene_renderer_data.m_fxaa_frame_buffer = frame_buffer::create({ 1920, 1080, {{"fxaa", fxaa_tex_spec}} });
	}

	void scene_renderer::create_screen_vao()
	{
		float squareVertices[5 * 4] = {
				1.0f, 1.0f, 0.0f, 1.0f, 1.0f,		// top right
				1.0f, -1.0f, 0.0f, 1.0f, 0.0f,	// bottom right
				-1.0f, -1.0f, 0.0f, 0.0f, 0.0f, // bottom left
				-1.0f, 1.0f, 0.0f, 0.0f, 1.0f		// top left
		};

		// Fill index buffer
		uint32_t squareIndices[6] = {
				0, 3, 1, // first triangle
				1, 3, 2, // second triangle
		};
		s_scene_renderer_data.m_screen_vao = vertex_array_buffer::create();
		const auto vbo = vertex_object_buffer::create(
			squareVertices, sizeof(squareVertices));
		const auto ibo = index_buffer::create(
			squareIndices, sizeof(squareIndices) / sizeof(uint32_t));
		s_scene_renderer_data.m_screen_vao->bind();
		vbo->set_layout({ {FloatVec3, "aPos"},
										 {FloatVec2, "aTexCoord"} });
		s_scene_renderer_data.m_screen_vao->add_vertex_buffer(vbo);
		s_scene_renderer_data.m_screen_vao->set_index_buffer(ibo);
		s_scene_renderer_data.m_screen_vao->un_bind();
	}

	void scene_renderer::create_camera(const shared<camera>& camera)
	{
		s_scene_renderer_data.m_camera = camera;
		s_scene_renderer_data.m_camera_ubo = uniform_buffer::create(sizeof(camera_data), 0);
	}

	void scene_renderer::setup_lights()
	{
		s_scene_renderer_data.m_pointLight.color = glm::vec4(0);
		s_scene_renderer_data.m_pointLight.position = glm::vec4(0);
		s_scene_renderer_data.m_pointLight.radius = 0;
		s_scene_renderer_data.m_pointLight.intensity = 0;

		s_scene_renderer_data.m_directional_light.color = glm::vec4(0);
		s_scene_renderer_data.m_directional_light.direction = glm::vec4(0);
		s_scene_renderer_data.m_directional_light.intensity = 0;

		s_scene_renderer_data.m_lights_ubo = uniform_buffer::create(
			sizeof(point_light_data) + sizeof(directional_light_data), 1);
	}

	void scene_renderer::setup_environment()
	{
		const shared<texture_cubemap>& sky_cubemap = texture_cubemap::create({ "Assets/Textures/HDR/drakensberg_solitary_mountain_4k.hdr",
																																					texture_filtering::linear,
																																					texture_wrapping::clamp_edge });
		s_scene_renderer_data.m_lighting_environment = lighting_environment::create(
			{ sky_cubemap, 1024, 256, 1024, 1024 });
	}
}