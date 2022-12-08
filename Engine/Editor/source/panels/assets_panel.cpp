#include "assets_panel.h"

#include "imgui.h"
#include "../editor_interface_utils.h"
#include "core/application/retro_application.h"

namespace retro::editor
{
	assets_panel::assets_panel()
	{
	}

	assets_panel::~assets_panel()
	{
	}

	void assets_panel::on_update_panel(float time)
	{
	}

	void assets_panel::on_render_panel()
	{
		const retro_application& application = retro_application::get_application();
		ImGui::Begin("Assets");
		ImGuiTreeNodeFlags flags = ImGuiTreeNodeFlags_OpenOnArrow;
		flags |= ImGuiTreeNodeFlags_SpanAvailWidth;
		ImGui::Text("Loaded assets");
		ImGui::Separator();
		for (const auto& asset_type : application.get_assets_manager()->get_assets())
		{
			if (ImGui::TreeNodeEx(
				reinterpret_cast<void*>(static_cast<uint64_t>(static_cast<uint32_t>(asset_type.first))),
				flags, asset::get_asset_to_string(asset_type.first).c_str()))
			{
				for (const auto& asset : asset_type.second)
				{
					std::string label = "UUID: " + std::to_string(asset.second->get_uuid()->get_uuid());
					if (ImGui::TreeNodeEx(
						reinterpret_cast<void*>(asset.first->get_uuid()),
						flags, label.c_str()))
					{
						if (asset_type.first == asset_type::texture)
						{
							if (const shared<renderer::texture>& texture = std::dynamic_pointer_cast<renderer::texture>(asset.second)) {
								editor_interface_utils::draw_property(texture);
							}
						}
						else if (asset_type.first == asset_type::model)
						{
							if (const shared<renderer::model>& model = std::dynamic_pointer_cast<renderer::model>(asset.second)) {
								ImGui::Text("Rendereable Count: %d", model->get_model_renderables().size());
							}
						}
						ImGui::TreePop();
					}
				}
				ImGui::TreePop();
			}
		}

		ImGui::End();
	}
}