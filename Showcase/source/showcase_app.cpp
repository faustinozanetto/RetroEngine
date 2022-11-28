#pragma once

#include <ranges>
#include <glm/gtc/type_ptr.hpp>

#include "imgui.h"
#include "core/application/retro_application.h"
#include "core/interfaces/interface_layer.h"
#include "core/layers/layer.h"
#include "core/entry_point.h"
#include "core/scene/actor.h"
#include "core/scene/components.h"
#include "renderer/camera/camera.h"
#include "renderer/lighting/point_light.h"
#include "renderer/materials/material.h"
#include "renderer/rendereables/model/model.h"
#include "renderer/renderer/renderer.h"
#include "renderer/renderer/scene_renderer.h"

class showcase_layer : public retro::layer
{
public:
	showcase_layer() : layer("Sandbox Layer")
	{
		m_Scene = retro::scene::create("Test Scene");
		//m_LightModel = retro::renderer::model::create("Assets/Models/Cube.obj");

		/*
		{
			auto albedo = retro::renderer::texture::create({
				"Assets/Models/Cerberus/textures/Cerberus_A.png",
				retro::renderer::texture_filtering::linear,
				retro::renderer::texture_wrapping::clamp_edge,
			});
			retro::renderer::material_texture albedoTexture = {
				albedo, true
			};
			auto normal = retro::renderer::texture::create({
				"Assets/Models/Cerberus/textures/Cerberus_N.png",
				retro::renderer::texture_filtering::linear,
				retro::renderer::texture_wrapping::clamp_edge,
			});
			retro::renderer::material_texture normalTexture = {
				normal, true
			};
			auto roughness = retro::renderer::texture::create({
				"Assets/Models/Cerberus/textures/Cerberus_R.png",
				retro::renderer::texture_filtering::linear,
				retro::renderer::texture_wrapping::clamp_edge,
			});
			retro::renderer::material_texture roughnessTexture = {
				roughness, true
			};
			auto metallic = retro::renderer::texture::create({
				"Assets/Models/Cerberus/textures/Cerberus_M.png",
				retro::renderer::texture_filtering::linear,
				retro::renderer::texture_wrapping::clamp_edge,
			});
			retro::renderer::material_texture metallicTexture = {
				metallic, true
			};
			auto ambient_occlusion = retro::renderer::texture::create({
				"Assets/Models/Cerberus/textures/Cerberus_AO.png",
				retro::renderer::texture_filtering::linear,
				retro::renderer::texture_wrapping::clamp_edge,
			});
			retro::renderer::material_texture ambient_occlusion_texture = {
				ambient_occlusion, true
			};
			const std::map<retro::renderer::material_texture_type, retro::renderer::material_texture> textures = {
				{retro::renderer::material_texture_type::albedo, albedoTexture},
				{retro::renderer::material_texture_type::normal, normalTexture},
				{retro::renderer::material_texture_type::metallic, roughnessTexture},
				{retro::renderer::material_texture_type::roughness, metallicTexture},
				{retro::renderer::material_texture_type::ambient_occlusion, ambient_occlusion_texture}
			};
			const retro::renderer::material_specification materialSpecification = {
				textures,
				glm::vec4(1.0f, 0.23f, 0.5f, 1.0f),
				1.0f,
				1.0f,
			};
			auto mat = retro::renderer::material::create(
				materialSpecification);
			auto model = retro::renderer::model::create("Assets/Models/Cerberus/source/Cerberus_LP.FBX.fbx");
			auto cerberus = m_Scene->create_actor();
			cerberus->add_component<retro::name_component>("Cerberus");
			cerberus->add_component<retro::model_renderer_component>(model);
			cerberus->add_component<retro::material_component>(mat);
			auto& transform = cerberus->add_component<retro::transform_component>();
			transform.scale = glm::vec3(0.1f);
			transform.rotation = glm::vec3(-1.55f, -4.0f, 0.0f);
			transform.position = glm::vec3(-8.5f, -0.3f, 0.0f);
		}
*/
		{
			auto albedo = retro::renderer::texture::create({
				"Assets/Models/SciFiHelmet/glTF/SciFiHelmet_BaseColor.png",
				retro::renderer::texture_filtering::linear,
				retro::renderer::texture_wrapping::clamp_edge,
				});
			retro::renderer::material_texture albedoTexture = {
				albedo, true
			};
			auto normal = retro::renderer::texture::create({
				"Assets/Models/SciFiHelmet/glTF/SciFiHelmet_Normal.png",
				retro::renderer::texture_filtering::linear,
				retro::renderer::texture_wrapping::clamp_edge,
				});
			retro::renderer::material_texture normalTexture = {
				normal, true
			};
			auto roughness = retro::renderer::texture::create({
				"Assets/Models/SciFiHelmet/glTF/SciFiHelmet_MetallicRoughness.png",
				retro::renderer::texture_filtering::linear,
				retro::renderer::texture_wrapping::clamp_edge,
				});
			retro::renderer::material_texture roughnessTexture = {
				roughness, true
			};
			auto metallic = retro::renderer::texture::create({
				"Assets/Models/SciFiHelmet/glTF/SciFiHelmet_MetallicRoughness.png",
				retro::renderer::texture_filtering::linear,
				retro::renderer::texture_wrapping::clamp_edge,
				});
			retro::renderer::material_texture metallicTexture = {
				metallic, true
			};
			auto ambient_occlusion = retro::renderer::texture::create({
				"Assets/Models/SciFiHelmet/glTF/SciFiHelmet_AmbientOcclusion.png",
				retro::renderer::texture_filtering::linear,
				retro::renderer::texture_wrapping::clamp_edge,
				});
			retro::renderer::material_texture ambient_occlusion_texture = {
				ambient_occlusion, true
			};
			const std::map<retro::renderer::material_texture_type, retro::renderer::material_texture> textures = {
				{retro::renderer::material_texture_type::albedo, albedoTexture},
				{retro::renderer::material_texture_type::normal, normalTexture},
				{retro::renderer::material_texture_type::metallic, roughnessTexture},
				{retro::renderer::material_texture_type::roughness, metallicTexture},
				{retro::renderer::material_texture_type::ambient_occlusion, ambient_occlusion_texture}
			};
			const retro::renderer::material_specification materialSpecification = {
				textures,
				glm::vec4(1.0f, 0.23f, 0.5f, 1.0f),
				1.0f,
				1.0f,
			};
			auto material = retro::renderer::material::create(
				materialSpecification);
			auto sphere = m_Scene->create_actor();
			sphere->add_component<retro::name_component>("SciFiHelmet");
			auto sphereModel = retro::renderer::model::create({ "Assets/Models/SciFiHelmet/glTF/SciFiHelmet.obj" });
			sphere->add_component<retro::model_renderer_component>(sphereModel);
			sphere->add_component<retro::material_component>(material);
			sphere->add_component<retro::transform_component>();
		}

		auto light = m_Scene->create_actor();
		light->add_component<retro::name_component>("Point Light");
		light->add_component<retro::model_renderer_component>(m_LightModel);
		light->add_component<retro::transform_component>();
		auto point_light = retro::create_shared<retro::renderer::point_light>();
		light->add_component<retro::light_renderer_component>(point_light, retro::light_type::point);
		m_camera = retro::create_shared<retro::renderer::camera>(50.0f, 0.01f, 2000.0f);

		retro::renderer::scene_renderer::set_scene(m_Scene);
		retro::renderer::scene_renderer::initialize(m_camera);
	}

	void on_layer_registered() override
	{
	}

	void on_layer_unregistered() override
	{
	}

	void on_layer_updated() override
	{
		m_camera->set_fov(m_fov);
		retro::renderer::scene_renderer::begin_render();

		retro::renderer::scene_renderer::end_render();

		// Setup dockspace
		{
			bool open = true;
			const ImGuiDockNodeFlags dockspaceFlags = ImGuiDockNodeFlags_None;
			ImGuiWindowFlags windowFlags = ImGuiWindowFlags_MenuBar | ImGuiWindowFlags_NoDocking;
			const ImGuiViewport* viewport = ImGui::GetMainViewport();
			ImGui::SetNextWindowPos(viewport->Pos);
			ImGui::SetNextWindowSize(viewport->Size);
			ImGui::SetNextWindowViewport(viewport->ID);
			windowFlags |= ImGuiWindowFlags_NoTitleBar | ImGuiWindowFlags_NoCollapse | ImGuiWindowFlags_NoResize |
				ImGuiWindowFlags_NoMove;
			windowFlags |= ImGuiWindowFlags_NoBringToFrontOnFocus | ImGuiWindowFlags_NoNavFocus;

			ImGui::Begin("DockSpace", &open, windowFlags);

			// DockSpace
			const ImGuiIO& io = ImGui::GetIO();
			ImGuiStyle& style = ImGui::GetStyle();
			const float minWinSizeX = style.WindowMinSize.x;
			style.WindowMinSize.x = 370.0f;
			if (io.ConfigFlags & ImGuiConfigFlags_DockingEnable)
			{
				const ImGuiID dockspace_id = ImGui::GetID("MyDockSpace");
				ImGui::DockSpace(dockspace_id, ImVec2(0.0f, 0.0f), dockspaceFlags);
			}
			style.WindowMinSize.x = minWinSizeX;
		}

		const auto view = m_Scene->get_actor_registry().group<retro::model_renderer_component>
			(entt::get<retro::name_component, retro::transform_component>);
		ImGui::Begin("Actors");
		if (ImGui::TreeNode("List"))
		{
			for (auto&& [actor, model_renderer, name, transform] : view.each())
			{
				if (ImGui::TreeNode(reinterpret_cast<void*>(static_cast<intptr_t>(actor)), "Asset %s",
					name.name.c_str()))
				{
					{
						ImGui::TextUnformatted("Scale");
						ImGui::NextColumn();
						ImGui::PushItemWidth(-1);
						const std::string id = "##scale";
						if (ImGui::DragFloat3(id.c_str(), glm::value_ptr(transform.scale), 0.1f));
						ImGui::PopItemWidth();
						ImGui::NextColumn();
					}

					{
						ImGui::TextUnformatted("Position");
						ImGui::NextColumn();
						ImGui::PushItemWidth(-1);
						const std::string id = "##Position";
						if (ImGui::DragFloat3(id.c_str(), glm::value_ptr(transform.position), 0.1f));
						ImGui::PopItemWidth();
						ImGui::NextColumn();
					}

					{
						ImGui::TextUnformatted("Rotation");
						ImGui::NextColumn();
						ImGui::PushItemWidth(-1);
						const std::string id = "##Rotation";
						if (ImGui::DragFloat3(id.c_str(), glm::value_ptr(transform.rotation), 0.1f));
						ImGui::PopItemWidth();
						ImGui::NextColumn();
					}

					if (m_Scene->get_actor_registry().has<retro::material_component>(actor))
					{
						auto& material_component = m_Scene->get_actor_registry().get<retro::material_component>(actor);
						for (auto& texture : material_component.material->get_material_specification().textures)
						{
							auto label = "Enabled " + retro::renderer::material::get_texture_type_to_string(
								texture.first);
							ImGui::Checkbox(label.c_str(), &texture.second.enabled);
						}
						ImGui::ColorPicker4(
							"Albedo", glm::value_ptr(material_component.material->get_material_specification().albedo));
						ImGui::SliderFloat("Roughness",
							&material_component.material->get_material_specification().roughness, 0.0f,
							1.0f);
						ImGui::SliderFloat(
							"Metallic", &material_component.material->get_material_specification().metallic, 0.0f,
							1.0f);
					}

					ImGui::TreePop();
				}
			}
			ImGui::TreePop();
		}
		ImGui::End();

		ImGui::Begin("Camera");
		ImGui::SliderFloat3("Camera Pos", value_ptr(m_camera_pos), -10.0f, 10.0f);
		ImGui::SliderFloat("Camera FOV", &m_fov, 1.0f, 90.0f);
		ImGui::End();

		/*
		ImGui::Begin("Assets");

		if (ImGui::TreeNode("List"))
		{
			for (auto& assetsCategory : retro::retro_application::get_application().get_assets_manager()->get_assets())
			{
				if (ImGui::TreeNode(reinterpret_cast<void*>(static_cast<intptr_t>(assetsCategory.first)), "Asset %s",
									retro::asset::get_asset_to_string(assetsCategory.first).c_str()))
				{
					for (const auto& key : assetsCategory.second | std::views::keys)
					{
						ImGui::Text("UUID: %lld", key->get());
					}
					ImGui::TreePop();
				}
			}
			ImGui::TreePop();
		}
		ImGui::End();

		ImGui::Begin("Actors");

		if (ImGui::TreeNode("List"))
		{
			int i = 0;
			for (auto& actor : m_Scene->get_actor_registry().view<retro::name_component>())
			{
				if (ImGui::TreeNode(reinterpret_cast<void*>(i), "Actor %d", i))
				{
					ImGui::Text(m_Scene->get_actor_registry().get<retro::name_component>(actor).name.c_str());
					ImGui::TreePop();
				}
				i++;
			}
			ImGui::TreePop();
		}
		ImGui::End();

*/

		ImGui::Begin("Viewport");
		const auto viewportMinRegin = ImGui::GetWindowContentRegionMin();
		const auto viewportMaxRegin = ImGui::GetWindowContentRegionMax();
		const auto viewportOffset = ImGui::GetWindowPos();

		// Viewport bounds
		m_ViewportBounds[0] = { viewportMinRegin.x + viewportOffset.x, viewportMinRegin.y + viewportOffset.y };
		m_ViewportBounds[1] = { viewportMaxRegin.x + viewportOffset.x, viewportMaxRegin.y + viewportOffset.y };

		ImVec2 viewportPanelSize = ImGui::GetContentRegionAvail();
		m_ViewportSize = { viewportPanelSize.x, viewportPanelSize.y };

		// Draw viewport
		ImGui::Image(
			reinterpret_cast<ImTextureID>(retro::renderer::scene_renderer::get_final_texture()),
			ImVec2{ m_ViewportSize.x, m_ViewportSize.y }, ImVec2{ 0, 1 },
			ImVec2{ 1, 0 });

		ImGui::End();
		ImGui::End();
	}

private:
	glm::vec3 m_camera_pos = glm::vec3(1.0f);
	float m_fov = 50.0f;
	glm::vec2 m_ViewportBounds[2];
	glm::vec2 m_ViewportSize = { 1920.0f, 1080.0f };
	retro::shared<retro::scene> m_Scene;
	retro::shared<retro::renderer::camera> m_camera;
	retro::shared<retro::renderer::model> m_LightModel;
};

class showcase_interface_layer : public retro::interface_layer
{
public:
	showcase_interface_layer() : interface_layer("Showcase Interface Layer")
	{
		m_use_vsync = retro::retro_application::get_application().get_window()->is_vsync_enabled();
	}

	void on_interface_renderer() override
	{
		ImGui::ShowDemoWindow();
		ImGui::Begin("Sandbox");
		const float frameTime = 1000.0f / ImGui::GetIO().Framerate;
		ImGui::Text("Frame time: %.3f ms", frameTime, ImGui::GetIO().Framerate);
		ImGui::Text("Frame rate: %.3f FPS", ImGui::GetIO().Framerate);
		ImGui::End();

		ImGui::Begin("Renderer");
		if (ImGui::Button("Normal"))
		{
			retro::renderer::renderer::set_renderer_mode(retro::renderer::renderer_mode::normal);
		}
		if (ImGui::Button("Wireframe"))
		{
			retro::renderer::renderer::set_renderer_mode(retro::renderer::renderer_mode::wireframe);
		}
		if (ImGui::Button("Point"))
		{
			retro::renderer::renderer::set_renderer_mode(retro::renderer::renderer_mode::point);
		}
		if (ImGui::Checkbox("VSync", &m_use_vsync))
		{
			retro::retro_application::get_application().get_window()->set_vsync_enabled(m_use_vsync);
		}
		ImGui::End();
	}

	void on_layer_registered() override
	{
	}

	void on_layer_unregistered() override
	{
	}

	void on_layer_updated() override
	{
	}

private:
	bool m_use_vsync;
};

class showcase_application : public retro::retro_application
{
public:
	showcase_application() : retro_application({ "Showcase" })
	{
		get_layers_manager()->register_layer(retro::create_shared<showcase_layer>());
		get_interfaces_layer_manager()->register_layer(retro::create_shared<showcase_interface_layer>());
	}

	~showcase_application() override
	{
	};
};

retro::retro_application* retro::create_retro_application(application_command_args command_args)
{
	return new showcase_application();
}