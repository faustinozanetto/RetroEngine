#include "render_passes_panel.h"

#include "imgui.h"
#include "renderer/renderer/scene_renderer.h"
#include "../editor_main_interface.h"

namespace retro::editor
{
	render_passes_panel::render_passes_panel()
	{
	}

	render_passes_panel::~render_passes_panel()
	{
	}

	void render_passes_panel::on_update_panel(float time)
	{
	}

	void render_passes_panel::on_render_panel()
	{
		ImGui::Begin("Render Passes");
		render_pass_details(renderer::scene_renderer::get_geometry_pass(), "Geometry Pass");
		ImGui::End();
	}

	void render_passes_panel::render_pass_details(const shared<renderer::render_pass>& render_pass,
		const std::string& name) const
	{
		if (ImGui::CollapsingHeader(name.c_str()))
		{
			for (const auto& [attachment_handle, attachment_specification] : render_pass->get_pass_output()->get_attachments_specifications())
			{
				if (ImGui::TreeNode(reinterpret_cast<void*>(static_cast<intptr_t>(attachment_handle)), "%s",
					attachment_specification.name.c_str()))
				{
					ImGui::PushStyleVar(ImGuiStyleVar_ItemSpacing, ImVec2{ 0, 5 });
					ImGui::Columns(2);
					ImGui::Text("Size %dx%d", attachment_specification.specification.size.x, attachment_specification.specification.size.y);

					ImGui::NextColumn();

					if (ImGui::ImageButton(
						reinterpret_cast<ImTextureID>(attachment_handle),
						{ 64.0f, 64.0f },
						ImVec2(0.0f, 1.0f),
						ImVec2(1.0f, 0.0f)))
					{
						editor_main_interface::s_render_target = attachment_handle;
					}
					if (ImGui::IsItemHovered())
					{
						ImGui::BeginTooltip();
						ImGui::Image(
							reinterpret_cast<ImTextureID>(attachment_handle),
							{ 256.0f, 256.0f },
							ImVec2(0.0f, 1.0f),
							ImVec2(1.0f, 0.0f));
						ImGui::EndTooltip();
					}
					ImGui::Columns(1);
					ImGui::PopStyleVar();
					ImGui::TreePop();
				}
			}
		}
	}
}