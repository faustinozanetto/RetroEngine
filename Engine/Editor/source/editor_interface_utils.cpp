#include "editor_interface_utils.h"

#include <glm/gtc/type_ptr.hpp>

#include "editor_main_interface.h"
#include "imgui.h"
#include "imgui_internal.h"
#include "core/application/retro_application.h"
#include "renderer/texture/texture.h"

namespace retro::editor
{
	void editor_interface_utils::draw_property(const std::string& name)
	{
		ImGui::TextUnformatted(name.c_str());
		ImGui::NextColumn();
		ImGui::PushItemWidth(-1);
		ImGui::PopItemWidth();
		ImGui::NextColumn();
	}

	void editor_interface_utils::draw_property(const std::string& name, const std::string& content)
	{
		ImGui::PushID(name.c_str());
		ImGui::Columns(2);
		ImGui::SetColumnWidth(0, 200.0f);
		ImGui::PushStyleVar(ImGuiStyleVar_ItemSpacing, ImVec2{ 0, 5 });
		ImGui::AlignTextToFramePadding();
		ImGui::Text(name.c_str());
		ImGui::PopStyleVar();
		ImGui::NextColumn();

		ImGui::Text(content.c_str());

		ImGui::Columns(1);
		ImGui::PopID();
	}

	bool editor_interface_utils::draw_property(const std::string& name, bool& value)
	{
		bool modified = false;
		ImGui::PushID(name.c_str());
		ImGui::Columns(2);
		ImGui::SetColumnWidth(0, 200.0f);
		ImGui::PushStyleVar(ImGuiStyleVar_ItemSpacing, ImVec2{ 0, 5 });
		ImGui::AlignTextToFramePadding();
		ImGui::Text(name.c_str());
		ImGui::PopStyleVar();
		ImGui::NextColumn();

		const std::string id = "##" + name;
		if (ImGui::Checkbox(id.c_str(), &value))
		{
			modified = true;
		}

		ImGui::Columns(1);
		ImGui::PopID();

		return modified;
	}

	bool editor_interface_utils::draw_property(const std::string& name, int& value, int min, int max)
	{
		bool modified = false;
		ImGui::PushID(name.c_str());
		ImGui::Columns(2);
		ImGui::SetColumnWidth(0, 200.0f);
		ImGui::PushStyleVar(ImGuiStyleVar_ItemSpacing, ImVec2{ 0, 5 });
		ImGui::AlignTextToFramePadding();
		ImGui::Text(name.c_str());
		ImGui::PopStyleVar();
		ImGui::NextColumn();

		const std::string id = "##" + name;
		if (ImGui::SliderInt(id.c_str(), &value, min, max))
		{
			modified = true;
		}

		ImGui::Columns(1);
		ImGui::PopID();

		return modified;
	}

	bool editor_interface_utils::draw_property(const std::string& name, float& value, float min, float max, float step)
	{
		bool modified = false;
		ImGui::PushID(name.c_str());
		ImGui::Columns(2);
		ImGui::SetColumnWidth(0, 200.0f);
		ImGui::PushStyleVar(ImGuiStyleVar_ItemSpacing, ImVec2{ 0, 5 });
		ImGui::AlignTextToFramePadding();
		ImGui::Text(name.c_str());
		ImGui::PopStyleVar();
		ImGui::NextColumn();

		const std::string id = "##" + name;
		if (ImGui::DragFloat(id.c_str(), &value, step, min, max))
		{
			modified = true;
		}

		ImGui::Columns(1);
		ImGui::PopID();

		return modified;
	}

	bool editor_interface_utils::draw_property(const std::string& name, glm::vec2& value, float min, float max,
		float step)
	{
		bool modified = false;
		ImGui::PushID(name.c_str());
		ImGui::Columns(2);
		ImGui::SetColumnWidth(0, 200.0f);
		ImGui::PushStyleVar(ImGuiStyleVar_ItemSpacing, ImVec2{ 0, 5 });
		ImGui::AlignTextToFramePadding();
		ImGui::Text(name.c_str());
		ImGui::PopStyleVar();
		ImGui::NextColumn();

		ImGui::PushMultiItemsWidths(2, ImGui::CalcItemWidth());
		ImGui::PushStyleVar(ImGuiStyleVar_ItemSpacing, ImVec2{ 0, 5 });

		float lineHeight = ImGui::GetTextLineHeightWithSpacing();
		ImVec2 buttonSize = { lineHeight + 3.0f, lineHeight };

		ImGui::PushStyleColor(ImGuiCol_Button, ImVec4{ 0.8f, 0.1f, 0.15f, 1.0f });
		ImGui::PushStyleColor(ImGuiCol_ButtonHovered, ImVec4{ 0.9f, 0.2f, 0.2f, 1.0f });
		ImGui::PushStyleColor(ImGuiCol_ButtonActive, ImVec4{ 0.8f, 0.1f, 0.15f, 1.0f });
		if (ImGui::Button("X", buttonSize))
		{
			value.x = 0.0f;
			modified = true;
		}
		ImGui::PopStyleColor(3);

		ImGui::SameLine();
		if (ImGui::DragFloat("##X", &value.x, step, min, max, "%.2f"))
			modified = true;
		ImGui::PopItemWidth();
		ImGui::SameLine();

		ImGui::PushStyleColor(ImGuiCol_Button, ImVec4{ 0.2f, 0.7f, 0.2f, 1.0f });
		ImGui::PushStyleColor(ImGuiCol_ButtonHovered, ImVec4{ 0.3f, 0.8f, 0.3f, 1.0f });
		ImGui::PushStyleColor(ImGuiCol_ButtonActive, ImVec4{ 0.2f, 0.7f, 0.2f, 1.0f });
		if (ImGui::Button("Y", buttonSize))
		{
			value.y = 0.0f;
			modified = true;
		}
		ImGui::PopStyleColor(3);

		ImGui::SameLine();
		if (ImGui::DragFloat("##Y", &value.y, step, min, max, "%.2f"))
			modified = true;
		ImGui::PopItemWidth();
		ImGui::SameLine();

		ImGui::PopStyleVar();

		ImGui::Columns(1);

		ImGui::PopID();
		return modified;
	}

	void editor_interface_utils::draw_property(const std::string& name, glm::vec3& value, float min, float max,
		float step)
	{
		ImGui::PushID(name.c_str());
		ImGui::Columns(2);
		ImGui::SetColumnWidth(0, 200.0f);
		ImGui::PushStyleVar(ImGuiStyleVar_ItemSpacing, ImVec2{ 0, 5 });
		ImGui::AlignTextToFramePadding();
		ImGui::Text(name.c_str());
		ImGui::PopStyleVar();
		ImGui::NextColumn();

		ImGui::PushMultiItemsWidths(3, ImGui::CalcItemWidth());
		ImGui::PushStyleVar(ImGuiStyleVar_ItemSpacing, ImVec2{ 0, 5 });

		float lineHeight = ImGui::GetTextLineHeightWithSpacing();
		ImVec2 buttonSize = { lineHeight + 3.0f, lineHeight };

		ImGui::PushStyleColor(ImGuiCol_Button, ImVec4{ 0.8f, 0.1f, 0.15f, 1.0f });
		ImGui::PushStyleColor(ImGuiCol_ButtonHovered, ImVec4{ 0.9f, 0.2f, 0.2f, 1.0f });
		ImGui::PushStyleColor(ImGuiCol_ButtonActive, ImVec4{ 0.8f, 0.1f, 0.15f, 1.0f });
		if (ImGui::Button("X", buttonSize))
			value.x = 0.0f;
		ImGui::PopStyleColor(3);

		ImGui::SameLine();
		ImGui::DragFloat("##X", &value.x, step, min, max, "%.2f");
		ImGui::PopItemWidth();
		ImGui::SameLine();

		ImGui::PushStyleColor(ImGuiCol_Button, ImVec4{ 0.2f, 0.7f, 0.2f, 1.0f });
		ImGui::PushStyleColor(ImGuiCol_ButtonHovered, ImVec4{ 0.3f, 0.8f, 0.3f, 1.0f });
		ImGui::PushStyleColor(ImGuiCol_ButtonActive, ImVec4{ 0.2f, 0.7f, 0.2f, 1.0f });
		if (ImGui::Button("Y", buttonSize))
			value.y = 0.0f;
		ImGui::PopStyleColor(3);

		ImGui::SameLine();
		ImGui::DragFloat("##Y", &value.y, step, min, max, "%.2f");
		ImGui::PopItemWidth();
		ImGui::SameLine();

		ImGui::PushStyleColor(ImGuiCol_Button, ImVec4{ 0.1f, 0.25f, 0.8f, 1.0f });
		ImGui::PushStyleColor(ImGuiCol_ButtonHovered, ImVec4{ 0.2f, 0.35f, 0.9f, 1.0f });
		ImGui::PushStyleColor(ImGuiCol_ButtonActive, ImVec4{ 0.1f, 0.25f, 0.8f, 1.0f });
		if (ImGui::Button("Z", buttonSize))
			value.z = 0.0f;
		ImGui::PopStyleColor(3);

		ImGui::SameLine();
		ImGui::DragFloat("##Z", &value.z, step, min, max, "%.2f");
		ImGui::PopItemWidth();

		ImGui::PopStyleVar();

		ImGui::Columns(1);

		ImGui::PopID();
	}

	bool editor_interface_utils::draw_property(const std::string& name, glm::vec3& value, bool color)
	{
		bool modified = false;
		ImGui::TextUnformatted(name.c_str());
		ImGui::NextColumn();
		ImGui::PushItemWidth(-1);

		const std::string id = "##" + name;
		if (ImGui::ColorEdit3(id.c_str(), glm::value_ptr(value)))
		{
			modified = true;
		}

		ImGui::PopItemWidth();
		ImGui::NextColumn();

		return modified;
	}

	bool editor_interface_utils::draw_property(const std::string& name, glm::vec4& value, bool color)
	{
		bool modified = false;
		ImGui::PushID(name.c_str());
		ImGui::Columns(2);
		ImGui::SetColumnWidth(0, 100.0f);
		ImGui::PushStyleVar(ImGuiStyleVar_ItemSpacing, ImVec2{ 0, 5 });
		ImGui::AlignTextToFramePadding();
		ImGui::Text(name.c_str());
		ImGui::PopStyleVar();
		ImGui::NextColumn();

		const std::string id = "##" + name;
		if (ImGui::ColorEdit4(id.c_str(), glm::value_ptr(value)))
		{
			modified = true;
		}

		ImGui::Columns(1);
		ImGui::PopID();

		return modified;
	}

	bool editor_interface_utils::draw_property(const shared<renderer::texture>& texture)
	{
		bool modified = false;
		ImGui::PushStyleVar(ImGuiStyleVar_ItemSpacing, ImVec2{ 0, 5 });
		if (texture) {
			ImGui::Columns(2);
			ImGui::Text("Size %dx%d", texture->get_width(), texture->get_height());
			ImGui::Text("Mip Maps: %d", texture->get_mip_maps_levels());
			ImGui::Text("Channels: %d", texture->get_channels());
			ImGui::Text("Path: %s", texture->get_texture_specification().path.c_str());
			if (ImGui::IsItemHovered())
			{
				ImGui::SetTooltip(texture->get_texture_specification().path.c_str());
			}

			ImGui::NextColumn();

			if (ImGui::ImageButton(
				reinterpret_cast<ImTextureID>(texture->get_object_handle()),
				{ 64.0f, 64.0f },
				ImVec2(0.0f, 1.0f),
				ImVec2(1.0f, 0.0f)))
			{
				modified = true;
			}
			if (ImGui::IsItemHovered())
			{
				ImGui::BeginTooltip();
				ImGui::Image(
					reinterpret_cast<ImTextureID>(texture->get_object_handle()),
					{ 256.0f, 256.0f },
					ImVec2(0.0f, 1.0f),
					ImVec2(1.0f, 0.0f));
				ImGui::EndTooltip();
			}
			ImGui::Columns(1);
		}
		else
		{
			if (ImGui::Button("Texture not found, load one?"))
			{
				modified = true;
			}
		}
		ImGui::PopStyleVar();
		return modified;
	}

	bool editor_interface_utils::draw_property(renderer::material_texture& material_texture)
	{
		bool modified = false;
		draw_property("Enabled", material_texture.enabled);
		// If the texture property has been clicked, handle the file browser.
		if (draw_property(material_texture.mat_texture))
		{
			editor_main_interface::s_file_browser.SetTitle("Load texture");
			editor_main_interface::s_file_browser.Open();
		}
		editor_main_interface::s_file_browser.Display();
		// Handle the new texture update.
		if (editor_main_interface::s_file_browser.HasSelected())
		{
			std::string file_path = editor_main_interface::s_file_browser.GetSelected().string();
			const shared<renderer::texture>& tex = retro_application::get_application().get_assets_manager()->create_texture(
				{
					file_path, renderer::texture_filtering::linear, renderer::texture_wrapping::clamp_border
				});
			material_texture.mat_texture = tex;
			editor_main_interface::s_file_browser.ClearSelected();
		}
		return modified;
	}
}