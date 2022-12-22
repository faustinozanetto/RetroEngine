#include "pch.h"

#include "scene_renderer.h"

#include "renderer.h"
#include "core/scene/components.h"
#include "core/scene/actor.h"
#include "glad/glad.h"
#include "renderer/lighting/point_light.h"

#include <glm/glm.hpp>
#include <glm/gtc/type_ptr.hpp>
#include "core/assets/assets_manager.h"
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

		s_scene_renderer_data.fxaa_pass = create_shared<fxaa_pass>();
		s_scene_renderer_data.shadow_map_pass = create_shared<shadow_map_pass>();
		s_scene_renderer_data.geometry_pass = create_shared<geometry_pass>();
		s_scene_renderer_data.global_illumination_pass = create_shared<global_illumination_pass>();
		s_scene_renderer_data.ssao_pass = create_shared<ssao_pass>();
		s_scene_renderer_data.lighting_pass = create_shared<lighting_pass>();
		s_scene_renderer_data.lighting_pass->initialize_inputs();
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

		/* ==================== SHADOW PASS ==================== */
		s_scene_renderer_data.shadow_map_pass->begin_pass();

		/* ==================== GEOMETRY PASS ==================== */
		s_scene_renderer_data.geometry_pass->begin_pass();

		//s_scene_renderer_data.global_illumination_pass->begin_pass();
		if (s_scene_renderer_data.ssao_pass->render_pass_enabled()) {
			s_scene_renderer_data.ssao_pass->begin_pass();
		}
		/*==================== LIGHTING PASS ==================== */
		s_scene_renderer_data.lighting_pass->begin_pass();
		s_scene_renderer_data.lighting_pass->get_pass_output()->bind();
		/*==================== FINAL PASS ====================*/
		glBindFramebuffer(GL_READ_FRAMEBUFFER, s_scene_renderer_data.geometry_pass->get_pass_output()->get_object_handle());
		glBindFramebuffer(GL_DRAW_FRAMEBUFFER, s_scene_renderer_data.lighting_pass->get_pass_output()->get_object_handle());
		glBlitFramebuffer(0, 0, static_cast<int>(s_scene_renderer_data.lighting_pass->get_pass_output()->get_width()),
			static_cast<int>(s_scene_renderer_data.lighting_pass->get_pass_output()->get_height()), 0, 0,
			static_cast<int>(s_scene_renderer_data.lighting_pass->get_pass_output()->get_width()),
			static_cast<int>(s_scene_renderer_data.lighting_pass->get_pass_output()->get_height()),
			GL_DEPTH_BUFFER_BIT, GL_NEAREST);
		glEnable(GL_DEPTH_TEST);
		glDepthFunc(GL_LEQUAL);
		s_scene_renderer_data.m_lighting_environment->render_environment();
		glDepthFunc(GL_LESS);
		s_scene_renderer_data.lighting_pass->get_pass_output()->un_bind();

		if (s_scene_renderer_data.fxaa_pass->render_pass_enabled())
		{
			s_scene_renderer_data.fxaa_pass->begin_pass();
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
		return s_scene_renderer_data.lighting_pass->get_pass_output();
	}

	shared<lighting_environment>& scene_renderer::get_lighting_environment()
	{
		return s_scene_renderer_data.m_lighting_environment;
	}

	shared<shader>& scene_renderer::get_screen_shader()
	{
		return s_scene_renderer_data.m_screen_shader;
	}

	shared<camera>& scene_renderer::get_camera()
	{
		return s_scene_renderer_data.m_camera;
	}

	shared<vertex_array_buffer>& scene_renderer::get_screen_vao()
	{
		return s_scene_renderer_data.m_screen_vao;
	}

	scene_renderer_data& scene_renderer::get_data()
	{
		return s_scene_renderer_data;
	}

	uint32_t scene_renderer::get_final_texture()
	{
		return s_scene_renderer_data.lighting_pass->get_pass_output()->get_attachment_id(0);
	}

	shared<geometry_pass>& scene_renderer::get_geometry_pass()
	{
		return s_scene_renderer_data.geometry_pass;
	}

	shared<shadow_map_pass>& scene_renderer::get_shadow_pass()
	{
		return s_scene_renderer_data.shadow_map_pass;
	}

	shared<ssao_pass>& scene_renderer::get_ssao_pass()
	{
		return s_scene_renderer_data.ssao_pass;
	}

	shared<fxaa_pass>& scene_renderer::get_fxaa_pass()
	{
		return s_scene_renderer_data.fxaa_pass;
	}

	void scene_renderer::resize(uint32_t width, uint32_t height)
	{
		s_scene_renderer_data.ssao_pass->resize(width, height);
		s_scene_renderer_data.geometry_pass->resize(width, height);
		s_scene_renderer_data.lighting_pass->resize(width, height);
	}

	void scene_renderer::load_shaders()
	{
		s_scene_renderer_data.m_screen_shader = assets_manager::get().create_shader({ "Assets/Shaders/Screen/Screen.vert",
																													 "Assets/Shaders/Screen/Screen.frag" });
	}

	void scene_renderer::generate_frame_buffers()
	{
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