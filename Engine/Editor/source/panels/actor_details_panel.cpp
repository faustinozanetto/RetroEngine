#include "actor_details_panel.h"

#include <glm/gtc/type_ptr.hpp>

#include "imgui.h"
#include "../editor_interface_utils.h"
#include "../editor_main_interface.h"
#include "core/application/retro_application.h"
#include "core/scene/components.h"
#include "core/scene/scene.h"
#include "core/scene/actor.h"
#include "renderer/lighting/directional_light.h"
#include "renderer/lighting/point_light.h"

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
		const shared<scene>& active_scene = retro_application::get_application().get_scene_manager()->
			get_active_scene();
		ImGui::Begin("Actor Details");
		if (editor_main_interface::s_selected_actor != entt::null)
		{
			if (active_scene->get_actor_registry().has<name_component>(editor_main_interface::s_selected_actor))
			{
				auto& name_component = active_scene->get_actor_registry().get<retro::name_component>(
					editor_main_interface::s_selected_actor);
				// Editable name section.
				{
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
				auto& material_component = active_scene->get_actor_registry().get<retro::material_component>(
					editor_main_interface::s_selected_actor);
				editor_interface_utils::draw_property(
					"Albedo", material_component.material->get_material_specification().albedo, true);
				editor_interface_utils::draw_property(
					"Roughness", material_component.material->get_material_specification().roughness, 0.0f, 1.0f,
					0.01f);
				editor_interface_utils::draw_property(
					"Metallic", material_component.material->get_material_specification().metallic, 0.0f, 1.0f, 0.01f);
				editor_interface_utils::draw_property(
					"Ambient Occlusion", material_component.material->get_material_specification().ambient_occlusion,
					0.0f, 1.0f, 0.01f);
				for (auto& texture_type : material_component.material->get_material_specification().textures)
				{
					if (texture_type.second.mat_texture)
					{
						const std::string& label = renderer::material::get_texture_type_to_string(texture_type.first);
						ImGui::PushID(label.c_str());
						ImGui::AlignTextToFramePadding();
						if (ImGui::TreeNodeEx(label.c_str())) {
							editor_interface_utils::draw_property(texture_type.second.mat_texture);
							ImGui::TreePop();
						}
						ImGui::PopID();
					}
				}
			}
			ImGui::Separator();
			if (active_scene->get_actor_registry().has<model_renderer_component>(
				editor_main_interface::s_selected_actor))
			{
				const auto& model_renderer_component = active_scene->get_actor_registry().get<retro::model_renderer_component>(
					editor_main_interface::s_selected_actor);
				ImGui::Text("Renderables Count: %d", model_renderer_component.model->get_model_renderables().size());
				ImGuiTreeNodeFlags flags = ImGuiTreeNodeFlags_OpenOnArrow;
				flags |= ImGuiTreeNodeFlags_SpanAvailWidth;
				int renderable_idx = 0;
				if (ImGui::TreeNodeEx(model_renderer_component.model->get_uuid().get(), flags, "Rendereables"))
				{
					for (shared<renderer::renderable>& renderable : model_renderer_component.model->get_model_renderables()) {
						if (ImGui::TreeNodeEx(reinterpret_cast<void*>(renderable_idx), flags, "#%d", renderable_idx))
						{
							ImGui::Text("Vertices: %d", renderable->get_vertices().size());
							ImGui::Text("Indices: %d", renderable->get_indices().size());
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
				auto& light_renderer_component = active_scene->get_actor_registry().get<
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
						logger::info(
							"dir: " + std::to_string(directionalLight->get_direction().x) + " " + std::to_string(
								directionalLight->get_direction().y));
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