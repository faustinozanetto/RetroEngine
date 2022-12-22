#include "actor_details_panel.h"

#include <glm/gtc/type_ptr.hpp>

#include "imgui.h"
#include "../editor_interface_utils.h"
#include "../editor_main_interface.h"
#include "core/application/retro_application.h"
#include "core/assets/assets_manager.h"
#include "core/scene/components.h"
#include "core/scene/scene.h"
#include "core/scene/actor.h"
#include "renderer/lighting/directional_light.h"
#include "renderer/lighting/point_light.h"
#include "renderer/renderer/scene_renderer.h"

namespace retro::editor
{
	actor_details_panel::actor_details_panel()
	{
	}

	actor_details_panel::~actor_details_panel()
	{
	}

	void actor_details_panel::on_update_panel(float time)
	{
	}

	void actor_details_panel::on_render_panel()
	{
		const shared<scene>& active_scene = scene_manager::
			get_active_scene();
		ImGui::Begin("Actor Details");
		if (editor_main_interface::s_selected_actor != entt::null)
		{
			if (active_scene->get_actor_registry().has<name_component>(editor_main_interface::s_selected_actor))
			{
				// Editable name section.
				{
					auto& name_component = active_scene->get_actor_registry().get<retro::name_component>(
						editor_main_interface::s_selected_actor);
					char buffer[256] = {};
					strcpy_s(buffer, name_component.name.c_str());
					if (ImGui::InputText("##name", buffer, sizeof(buffer)))
					{
						name_component.name = std::string(buffer);
					}
				}
				// Actor ID section
				{
					ImGui::SameLine();
					ImGui::Text("Actor ID: %s",
						std::to_string(static_cast<uint32_t>(editor_main_interface::s_selected_actor)).c_str());
				}
			}
			ImGui::Separator();
			if (active_scene->get_actor_registry().has<transform_component>(editor_main_interface::s_selected_actor))
			{
				auto& transform_component = active_scene->get_actor_registry().get<retro::transform_component>(
					editor_main_interface::s_selected_actor);
				editor_interface_utils::draw_property("Position", transform_component.position, -10.0f, 10.0f, 0.05f);
				editor_interface_utils::draw_property("Rotation", transform_component.rotation, -10.0f, 10.0f, 0.05f);
				editor_interface_utils::draw_property("Scale", transform_component.scale, 0.0f, 10.0f, 0.05f);
			}
			ImGui::Separator();
			if (active_scene->get_actor_registry().has<material_component>(editor_main_interface::s_selected_actor))
			{
				material_component& material_component = active_scene->get_actor_registry().get<retro::material_component>(
					editor_main_interface::s_selected_actor);
				const auto& available_textures = assets_manager::get().get_assets_by_type(asset_type::texture);
				ImGuiTreeNodeFlags mat_flags = ImGuiTreeNodeFlags_OpenOnArrow;
				mat_flags |= ImGuiTreeNodeFlags_SpanAvailWidth;
				if (ImGui::TreeNodeEx("Materials")) {
					const std::map<int, shared<renderer::material>>& materials = material_component.materials;
					for (const auto& material : materials)
					{
						bool mat_node_open = ImGui::TreeNodeEx(material.second->get_material_specification().name.c_str(), mat_flags,
							"Material %s #%d", material.second->get_material_specification().name.c_str(), material.first);

						if (mat_node_open)
						{
							editor_interface_utils::draw_property(
								"Albedo", material.second->get_material_specification().albedo, true);
							editor_interface_utils::draw_property(
								"Roughness", material.second->get_material_specification().roughness, 0.0f, 1.0f,
								0.01f);
							editor_interface_utils::draw_property(
								"Metallic", material.second->get_material_specification().metallic, 0.0f, 1.0f, 0.01f);
							editor_interface_utils::draw_property(
								"Ambient Occlusion", material.second->get_material_specification().ambient_occlusion,
								0.0f, 1.0f, 0.01f);

							for (auto& texture_type : material.second->get_material_specification().textures)
							{
								const std::string& label = renderer::material::get_texture_type_to_string(texture_type.first);
								ImGui::PushID(label.c_str());
								ImGui::AlignTextToFramePadding();
								if (ImGui::TreeNodeEx(label.c_str())) {
									editor_interface_utils::draw_property(texture_type.second);
									// If there are loaded textures, show the combo box.
									if (!available_textures.empty()) {
										static uint64_t selected_tex_uuid = available_textures.begin()->first->get_uuid();
										if (ImGui::BeginCombo("##Select Existing Texture", std::to_string(selected_tex_uuid).c_str()))
										{
											for (const auto& tex : available_textures) {
												const bool is_selected = (selected_tex_uuid == tex.first->get_uuid());
												if (ImGui::Selectable(std::to_string(tex.first->get_uuid()).c_str(), is_selected)) {
													selected_tex_uuid = tex.first->get_uuid();
													texture_type.second.mat_texture = std::dynamic_pointer_cast<renderer::texture_2d>(tex.second);
												}
												// Set the initial focus when opening the combo (scrolling + keyboard navigation focus)
												if (is_selected)
													ImGui::SetItemDefaultFocus();
											}
											ImGui::EndCombo();
										}
									}
									ImGui::TreePop();
								}
								ImGui::PopID();
							}
							if (ImGui::Button("Delete"))
							{
								material_component.materials.erase(material.first);
							}

							ImGui::TreePop();
						}
					}
					if (ImGui::Button("Add Material"))
					{
						renderer::material_texture albedoTexture = {
							nullptr, false
						};
						renderer::material_texture normalTexture = {
							nullptr, false
						};
						renderer::material_texture metalRoughTexture = {
							nullptr, false
						};
						renderer::material_texture aoTexture = {
							nullptr, false
						};
						const std::map<renderer::material_texture_type, renderer::material_texture> mat_textures = {
							{renderer::material_texture_type::albedo, albedoTexture},
							{renderer::material_texture_type::normal, normalTexture},
							{renderer::material_texture_type::metallic, metalRoughTexture},
							{renderer::material_texture_type::roughness, metalRoughTexture},
							{renderer::material_texture_type::ambient_occlusion, aoTexture}
						};
						const renderer::material_specification material_specification = {
							mat_textures,
							"Material",
							glm::vec4(1.0f, 1.0f, 1.0f, 1.0f),
							1.0f,
							1.0f,
							1.0f,
						};
						const shared<renderer::material>& material = assets_manager::get().create_material(material_specification);
						auto new_mat_index = material_component.materials.size();
						material_component.materials.insert(std::pair(new_mat_index, material));
					}
					ImGui::TreePop();
				}
			}
			ImGui::Separator();
			if (active_scene->get_actor_registry().has<model_renderer_component>(
				editor_main_interface::s_selected_actor))
			{
				auto& model_renderer_component = active_scene->get_actor_registry().get<retro::model_renderer_component>(
					editor_main_interface::s_selected_actor);
				ImGui::Text("Rendereable Count: %llu", model_renderer_component.model->get_model_renderables().size());
				if (ImGui::Button("Load Model"))
				{
					editor_main_interface::s_file_browser.SetTitle("Load model");
					editor_main_interface::s_file_browser.Open();
				}
				editor_main_interface::s_file_browser.Display();
				// Handle the new texture update.
				if (editor_main_interface::s_file_browser.HasSelected())
				{
					std::string file_path = editor_main_interface::s_file_browser.GetSelected().string();
					const shared<renderer::model>& model = assets_manager::get().create_model(
						{
							file_path
						});
					model_renderer_component.model = model;
					editor_main_interface::s_file_browser.ClearSelected();
				}
				ImGuiTreeNodeFlags flags = ImGuiTreeNodeFlags_OpenOnArrow;
				flags |= ImGuiTreeNodeFlags_SpanAvailWidth;
				if (ImGui::TreeNodeEx(model_renderer_component.model->get_uuid().get(), flags, "Rendereables"))
				{
					int renderable_idx = 0;
					for (const shared<renderer::renderable>& renderable : model_renderer_component.model->get_model_renderables()) {
						bool rendereable_node_open = ImGui::TreeNodeEx((void*)(intptr_t)renderable_idx, flags, "%s", renderable->get_name().c_str());

						if (rendereable_node_open) {
							// If actor has a material_component, show the slider for the material index.
							if (active_scene->get_actor_registry().has<material_component>(editor_main_interface::s_selected_actor))
							{
								material_component& material_component = active_scene->get_actor_registry().get<retro::material_component>(
									editor_main_interface::s_selected_actor);
								if (!material_component.materials.empty()) {
									int mat_index = renderable->get_material_index();
									if (editor_interface_utils::draw_property("Material Index", mat_index, 0,
										material_component.materials.size() - 1))
									{
										renderable->set_material_index(mat_index);
									}
								}
							}
							ImGui::Text("Vertices: %llu", renderable->get_vertices().size());
							ImGui::Text("Indices: %llu", renderable->get_indices().size());
							ImGui::TreePop();
						}
						renderable_idx++;
					}
					ImGui::TreePop();
				}
			}
			ImGui::Separator();
			if (active_scene->get_actor_registry().has<light_renderer_component>(
				editor_main_interface::s_selected_actor))
			{
				const auto& light_renderer_component = active_scene->get_actor_registry().get<
					retro::light_renderer_component>(
						editor_main_interface::s_selected_actor);

				editor_interface_utils::draw_property("Color", light_renderer_component.light->get_color(), true);
				editor_interface_utils::draw_property("Intensity", light_renderer_component.light->get_intensity(),
					0.0f, 10.0f, 0.01f);

				// If type is directional, draw direction property.
				if (light_renderer_component.type == light_type::directional)
				{
					auto* directionalLight = dynamic_cast<renderer::directional_light*>(light_renderer_component.light.
						get());
					if (editor_interface_utils::draw_property("Direction", m_dir_light,
						-180.0, 180.0, 0.1f))
					{
						directionalLight->set_direction(m_dir_light.x, m_dir_light.y);
						renderer::scene_renderer::get_shadow_pass()->update_shadow_matrices(directionalLight->get_direction());
						renderer::scene_renderer::get_shadow_pass()->update_shadow_ubo();
					}
				}
				else if (light_renderer_component.type == light_type::point)
				{
					auto* point_light = dynamic_cast<renderer::point_light*>(light_renderer_component.light.
						get());
					editor_interface_utils::draw_property("Radius", point_light->get_radius(), 0.0f, 10.0f, 0.01f);
				}
			}
			ImGui::Separator();
		}
		else
		{
			ImGui::Text("No actor selected!");
		}
		ImGui::End();
	}
}