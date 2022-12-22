#include "pch.h"

#include "showcase_layer.h"

#include "imgui.h"
#include "core/application/retro_application.h"
#include "core/assets/assets_manager.h"
#include "core/input/input_manager.h"
#include "core/scene/actor.h"
#include "core/scene/components.h"
#include "renderer/renderer/renderer.h"
#include "renderer/renderer/scene_renderer.h"

namespace retro
{
	showcase_layer::showcase_layer(const std::string& name) : layer(name)
	{
	}

	showcase_layer::~showcase_layer()
	{
	}

	void showcase_layer::on_layer_registered()
	{
		const shared<scene>& scene = scene::create("Showcase Scene");
		scene_manager::set_active_scene(scene);
		renderer::camera_specification camera_specification = { 50.0f, 0.01f, 1000.0f };
		m_camera = retro::create_shared<renderer::camera>(camera_specification);

		m_shader = assets_manager::get().create_shader({ "Assets/Shaders/Screen/Screen.vert",
																											 "Assets/Shaders/Screen/Screen.frag" });
		create_showcase_model();
		create_screen_vao();

		renderer::scene_renderer::set_scene(
			scene_manager::get_active_scene());
		renderer::scene_renderer::initialize(m_camera);
		renderer::scene_renderer::get_ssao_pass()->set_render_pass_enabled(false);
		renderer::scene_renderer::get_fxaa_pass()->set_render_pass_enabled(false);
		retro_application::get_application().get_window()->set_vsync_enabled(false);
	}

	void showcase_layer::on_layer_unregistered()
	{
	}

	void showcase_layer::on_layer_updated()
	{
		renderer::scene_renderer::begin_render();
		renderer::scene_renderer::end_render();

		float cam_mov_amount = 0.05f;
		if (input::input_manager::is_key_pressed(input::key::W))
			m_camera->move(m_camera->get_position(), glm::conjugate(m_camera->get_orientation()) * glm::vec3(0, 0, -1),
				cam_mov_amount);
		if (input::input_manager::is_key_pressed(input::key::S))
			m_camera->move(m_camera->get_position(), glm::conjugate(m_camera->get_orientation()) * glm::vec3(0, 0, 1),
				cam_mov_amount);
		if (input::input_manager::is_key_pressed(input::key::A))
			m_camera->move(m_camera->get_position(), glm::conjugate(m_camera->get_orientation()) * glm::vec3(-1, 0, 0),
				cam_mov_amount);
		if (input::input_manager::is_key_pressed(input::key::D))
			m_camera->move(m_camera->get_position(), glm::conjugate(m_camera->get_orientation()) * glm::vec3(1, 0, 0),
				cam_mov_amount);
		if (input::input_manager::is_key_pressed(input::key::Q))
			m_camera->move(m_camera->get_position(), glm::conjugate(m_camera->get_orientation()) * glm::vec3(0, 1, 0),
				cam_mov_amount);
		if (input::input_manager::is_key_pressed(input::key::E))
			m_camera->move(m_camera->get_position(), glm::conjugate(m_camera->get_orientation()) * glm::vec3(0, -1, 0),
				cam_mov_amount);

		if (input::input_manager::is_mouse_button_pressed(input::key::ButtonRight))
		{
			if (!m_is_mouse_move)
			{
				m_mouse_pressed_position = input::input_manager::get_mouse_location();
			}
			m_is_mouse_move = true;
		}
		else
		{
			m_is_mouse_move = false;
		}
		if (m_is_mouse_move)
		{
			auto mouse_pos = input::input_manager::get_mouse_location();
			auto delta_pos = mouse_pos - m_mouse_pressed_position;

			auto rot_y = delta_pos.x != 0.0f;
			auto rot_x = delta_pos.y != 0.0f;

			/* pitch */
			if (rot_x)
			{
				m_camera->set_orientation(
					glm::angleAxis(glm::radians(delta_pos.y * 0.2f), glm::vec3(1, 0, 0)) * m_camera->get_orientation());
			}

			/* yaw */
			if (rot_y)
			{
				m_camera->set_orientation(
					m_camera->get_orientation() * glm::angleAxis(glm::radians(delta_pos.x * 0.2f), glm::vec3(0, 1, 0)));
			}

			if (rot_x || rot_y)
			{
				m_mouse_pressed_position = input::input_manager::get_mouse_location();
			}
		}

		renderer::renderer::set_renderer_state(renderer::renderer_state::depth_test, true);
		renderer::renderer::set_clear_color({ 0.0f, 0.0f, 0.0f, 1.0f });
		m_shader->bind();
		renderer::renderer::clear_screen();
		renderer::renderer::bind_texture(renderer::scene_renderer::get_final_texture(), 0);
		renderer::renderer::submit_command({ m_shader, m_screen_vao, nullptr });
		m_shader->un_bind();

		ImGui::Begin("Statistics");
		const float frameTime = 1000.0f / ImGui::GetIO().Framerate;
		ImGui::Text("Frame time: %.3f ms", frameTime, ImGui::GetIO().Framerate);
		ImGui::Text("Frame rate: %.3f FPS", ImGui::GetIO().Framerate);
		ImGui::Text("Application average %.3f ms/frame (%.1f FPS)", 1000.0f / ImGui::GetIO().Framerate, ImGui::GetIO().Framerate);
		ImGui::End();
	}

	void showcase_layer::create_screen_vao()
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
		m_screen_vao = renderer::vertex_array_buffer::create();
		const auto vbo = renderer::vertex_object_buffer::create(
			squareVertices, sizeof(squareVertices));
		const auto ibo = renderer::index_buffer::create(
			squareIndices, sizeof(squareIndices) / sizeof(uint32_t));
		m_screen_vao->bind();
		vbo->set_layout({ {renderer::FloatVec3, "aPos"},
										 {renderer::FloatVec2, "aTexCoord"} });
		m_screen_vao->add_vertex_buffer(vbo);
		m_screen_vao->set_index_buffer(ibo);
		m_screen_vao->un_bind();
	}

	void showcase_layer::create_showcase_model()
	{
		{
			auto albedo = renderer::texture_2d::create({
					"Assets/Models/FlightHelmet/glTF/FlightHelmet_Materials_RubberWoodMat_BaseColor.png",
					renderer::texture_filtering::linear,
					renderer::texture_wrapping::clamp_edge,
				});
			renderer::material_texture albedoTexture = {
				albedo, true
			};
			auto normal = renderer::texture_2d::create({
				"Assets/Models/FlightHelmet/glTF/FlightHelmet_Materials_RubberWoodMat_Normal.png",
				renderer::texture_filtering::linear,
				renderer::texture_wrapping::clamp_edge,
				});
			renderer::material_texture normalTexture = {
				normal, true
			};
			auto other = renderer::texture_2d::create({
				"Assets/Models/FlightHelmet/glTF/FlightHelmet_Materials_RubberWoodMat_OcclusionRoughMetal.png",
				renderer::texture_filtering::linear,
				renderer::texture_wrapping::clamp_edge,
				});
			renderer::material_texture otherTexture = {
				other, true
			};
			const std::map<renderer::material_texture_type, renderer::material_texture> textures = {
				{renderer::material_texture_type::albedo, albedoTexture},
				{renderer::material_texture_type::normal, normalTexture},
				{renderer::material_texture_type::metallic, otherTexture},
				{renderer::material_texture_type::roughness, otherTexture},
				{renderer::material_texture_type::ambient_occlusion, otherTexture}
			};
			const renderer::material_specification materialSpecification = {
				textures,
				"base_wooden",
				glm::vec4(1.0f, 0.23f, 0.5f, 1.0f),
				1.0f,
				1.0f,
			};
			auto baseWoodenMaterial = renderer::material::create(
				materialSpecification);
			auto baseWooden = scene_manager::get_active_scene()->
				create_actor();
			baseWooden->add_component<name_component>("FighterHelmet Base");
			auto model = baseWooden->add_component<model_renderer_component>(
				renderer::model::create({ "Assets/Models/FlightHelmet/glTF/WoodenBase.fbx" })).model;
			for (auto& model_rendereable : model->get_model_renderables())
			{
				model_rendereable->set_material_index(0);
			}
			baseWooden->add_component<material_component>().materials.insert(std::pair(0, baseWoodenMaterial));
			auto& transform = baseWooden->add_component<transform_component>();
			transform.scale = glm::vec3(0.1f);
			transform.position = { 0.0f, -0.4f, 9.0f };

			auto pipe = scene_manager::get_active_scene()->
				create_actor();
			pipe->add_component<name_component>("FighterHelmet Pipe");
			pipe->add_component<model_renderer_component>(
				renderer::model::create({ "Assets/Models/FlightHelmet/glTF/Pipe.fbx" }));
			pipe->add_component<material_component>().materials.insert(std::pair(0, baseWoodenMaterial));
			auto& transform2 = pipe->add_component<transform_component>();
			transform2.scale = glm::vec3(0.1f);
			transform2.position = { 0.0f, -0.4f, 9.0f };
		}
		{
			auto albedo = renderer::texture_2d::create({
				"Assets/Models/FlightHelmet/glTF/FlightHelmet_Materials_GlassPlasticMat_BaseColor.png",
				renderer::texture_filtering::linear,
				renderer::texture_wrapping::clamp_edge,
				});
			renderer::material_texture albedoTexture = {
				albedo, true
			};
			auto normal = renderer::texture_2d::create({
				"Assets/Models/FlightHelmet/glTF/FlightHelmet_Materials_GlassPlasticMat_Normal.png",
				renderer::texture_filtering::linear,
				renderer::texture_wrapping::clamp_edge,
				});
			renderer::material_texture normalTexture = {
				normal, true
			};
			auto other = renderer::texture_2d::create({
				"Assets/Models/FlightHelmet/glTF/FlightHelmet_Materials_GlassPlasticMat_OcclusionRoughMetal.png",
				renderer::texture_filtering::linear,
				renderer::texture_wrapping::clamp_edge,
				});
			renderer::material_texture otherTexture = {
				other, true
			};
			const std::map<renderer::material_texture_type, renderer::material_texture> textures = {
				{renderer::material_texture_type::albedo, albedoTexture},
				{renderer::material_texture_type::normal, normalTexture},
				{renderer::material_texture_type::metallic, otherTexture},
				{renderer::material_texture_type::roughness, otherTexture},
				{renderer::material_texture_type::ambient_occlusion, otherTexture}
			};
			const renderer::material_specification materialSpecification = {
				textures,
				"mask",
				glm::vec4(1.0f, 0.23f, 0.5f, 1.0f),
				1.0f,
				1.0f,
			};
			auto maskMat = renderer::material::create(
				materialSpecification);
			auto mask = scene_manager::get_active_scene()->
				create_actor();
			mask->add_component<name_component>("FighterHelmet Mask");
			mask->add_component<model_renderer_component>(
				renderer::model::create({ "Assets/Models/FlightHelmet/glTF/Mask.fbx" }));
			mask->add_component<material_component>().materials.insert(std::pair(0, maskMat));
			auto& transform = mask->add_component<transform_component>();
			transform.scale = glm::vec3(0.1f);
			transform.position = { 0.0f, -0.4f, 9.0f };
		}
		{
			auto albedo = renderer::texture_2d::create({
				"Assets/Models/FlightHelmet/glTF/FlightHelmet_Materials_LeatherPartsMat_BaseColor.png",
				renderer::texture_filtering::linear,
				renderer::texture_wrapping::clamp_edge,
				});
			renderer::material_texture albedoTexture = {
				albedo, true
			};
			auto normal = renderer::texture_2d::create({
				"Assets/Models/FlightHelmet/glTF/FlightHelmet_Materials_LeatherPartsMat_Normal.png",
				renderer::texture_filtering::linear,
				renderer::texture_wrapping::clamp_edge,
				});
			renderer::material_texture normalTexture = {
				normal, true
			};
			auto other = renderer::texture_2d::create({
				"Assets/Models/FlightHelmet/glTF/FlightHelmet_Materials_LeatherPartsMat_OcclusionRoughMetal.png",
				renderer::texture_filtering::linear,
				renderer::texture_wrapping::clamp_edge,
				});
			renderer::material_texture otherTexture = {
				other, true
			};
			const std::map<renderer::material_texture_type, renderer::material_texture> textures = {
				{renderer::material_texture_type::albedo, albedoTexture},
				{renderer::material_texture_type::normal, normalTexture},
				{renderer::material_texture_type::metallic, otherTexture},
				{renderer::material_texture_type::roughness, otherTexture},
				{renderer::material_texture_type::ambient_occlusion, otherTexture}
			};
			const renderer::material_specification materialSpecification = {
				textures,
				"helmet",
				glm::vec4(1.0f, 0.23f, 0.5f, 1.0f),
				1.0f,
				1.0f,
			};
			auto helmetMat = renderer::material::create(
				materialSpecification);
			auto helmet = scene_manager::get_active_scene()->
				create_actor();
			helmet->add_component<name_component>("FighterHelmet Helmet");
			helmet->add_component<model_renderer_component>(
				renderer::model::create({ "Assets/Models/FlightHelmet/glTF/Helmet.fbx" }));
			helmet->add_component<material_component>().materials.insert(std::pair(0, helmetMat));
			auto& transform = helmet->add_component<transform_component>();
			transform.scale = glm::vec3(0.1f);
			transform.position = { 0.0f, -0.4f, 9.0f };
		}
		{
			auto albedo = renderer::texture_2d::create({
				"Assets/Models/FlightHelmet/glTF/FlightHelmet_Materials_LensesMat_BaseColor.png",
				renderer::texture_filtering::linear,
				renderer::texture_wrapping::clamp_edge,
				});
			renderer::material_texture albedoTexture = {
				albedo, true
			};
			auto normal = renderer::texture_2d::create({
				"Assets/Models/FlightHelmet/glTF/FlightHelmet_Materials_LensesMat_Normal.png",
				renderer::texture_filtering::linear,
				renderer::texture_wrapping::clamp_edge,
				});
			renderer::material_texture normalTexture = {
				normal, true
			};
			auto other = renderer::texture_2d::create({
				"Assets/Models/FlightHelmet/glTF/FlightHelmet_Materials_LensesMat_OcclusionRoughMetal.png",
				renderer::texture_filtering::linear,
				renderer::texture_wrapping::clamp_edge,
				});
			renderer::material_texture otherTexture = {
				other, true
			};
			const std::map<renderer::material_texture_type, renderer::material_texture> textures = {
				{renderer::material_texture_type::albedo, albedoTexture},
				{renderer::material_texture_type::normal, normalTexture},
				{renderer::material_texture_type::metallic, otherTexture},
				{renderer::material_texture_type::roughness, otherTexture},
				{renderer::material_texture_type::ambient_occlusion, otherTexture}
			};
			const renderer::material_specification materialSpecification = {
				textures,
				"lenses",
				glm::vec4(1.0f, 0.23f, 0.5f, 1.0f),
				1.0f,
				1.0f,
			};
			auto lensesMat = renderer::material::create(
				materialSpecification);
			auto lenses = scene_manager::get_active_scene()->
				create_actor();
			lenses->add_component<name_component>("FighterHelmet Lenses");
			lenses->add_component<model_renderer_component>(
				renderer::model::create({ "Assets/Models/FlightHelmet/glTF/Lenses.fbx" }));
			lenses->add_component<material_component>().materials.insert(std::pair(0, lensesMat));
			auto& transform = lenses->add_component<transform_component>();
			transform.scale = glm::vec3(0.1f);
			transform.position = { 0.0f, -0.4f, 9.0f };
		}
		{
			auto albedo = renderer::texture_2d::create({
				"Assets/Models/FlightHelmet/glTF/FlightHelmet_Materials_MetalPartsMat_BaseColor.png",
				renderer::texture_filtering::linear,
				renderer::texture_wrapping::clamp_edge,
				});
			renderer::material_texture albedoTexture = {
				albedo, true
			};
			auto normal = renderer::texture_2d::create({
				"Assets/Models/FlightHelmet/glTF/FlightHelmet_Materials_MetalPartsMat_Normal.png",
				renderer::texture_filtering::linear,
				renderer::texture_wrapping::clamp_edge,
				});
			renderer::material_texture normalTexture = {
				normal, true
			};
			auto other = renderer::texture_2d::create({
				"Assets/Models/FlightHelmet/glTF/FlightHelmet_Materials_MetalPartsMat_OcclusionRoughMetal.png",
				renderer::texture_filtering::linear,
				renderer::texture_wrapping::clamp_edge,
				});
			renderer::material_texture otherTexture = {
				other, true
			};
			const std::map<renderer::material_texture_type, renderer::material_texture> textures = {
				{renderer::material_texture_type::albedo, albedoTexture},
				{renderer::material_texture_type::normal, normalTexture},
				{renderer::material_texture_type::metallic, otherTexture},
				{renderer::material_texture_type::roughness, otherTexture},
				{renderer::material_texture_type::ambient_occlusion, otherTexture}
			};
			const renderer::material_specification materialSpecification = {
				textures,
				"metals",
				glm::vec4(1.0f, 0.23f, 0.5f, 1.0f),
				1.0f,
				1.0f,
			};
			auto metalsMat = renderer::material::create(
				materialSpecification);
			auto metals = scene_manager::get_active_scene()->
				create_actor();
			metals->add_component<name_component>("FighterHelmet Metals");
			metals->add_component<model_renderer_component>(
				renderer::model::create({ "Assets/Models/FlightHelmet/glTF/MetalParts.fbx" }));
			metals->add_component<material_component>().materials.insert(std::pair(0, metalsMat));
			auto& transform = metals->add_component<transform_component>();
			transform.scale = glm::vec3(0.1f);
			transform.position = { 0.0f, -0.4f, 9.0f };
		}
	}
}