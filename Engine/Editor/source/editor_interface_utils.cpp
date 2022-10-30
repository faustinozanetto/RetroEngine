#include "editor_interface_utils.h"

#include <glm/gtc/type_ptr.hpp>

#include "imgui.h"
#include "imgui_internal.h"

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

    bool editor_interface_utils::draw_property(const std::string& name, bool& value)
    {
        bool modified = false;
        ImGui::TextUnformatted(name.c_str());
        ImGui::NextColumn();
        ImGui::PushItemWidth(-1);

        const std::string id = "##" + name;
        if (ImGui::Checkbox(id.c_str(), &value))
        {
            modified = true;
        }

        ImGui::PopItemWidth();
        ImGui::NextColumn();

        return modified;
    }

    bool editor_interface_utils::draw_property(const std::string& name, int& value, int min, int max, int step)
    {
        bool modified = false;
        ImGui::TextUnformatted(name.c_str());
        ImGui::NextColumn();
        ImGui::PushItemWidth(-1);

        const std::string id = "##" + name;
        if (ImGui::DragInt(id.c_str(), &value, step, min, max))
        {
            modified = true;
        }

        ImGui::PopItemWidth();
        ImGui::NextColumn();

        return modified;
    }

    bool editor_interface_utils::draw_property(const std::string& name, float& value, float min, float max, float step)
    {
        bool modified = false;
        ImGui::TextUnformatted(name.c_str());
        ImGui::NextColumn();
        ImGui::PushItemWidth(-1);

        const std::string id = "##" + name;
        if (ImGui::DragFloat(id.c_str(), &value, step, min, max))
        {
            modified = true;
        }

        ImGui::PopItemWidth();
        ImGui::NextColumn();

        return modified;
    }

    void editor_interface_utils::draw_property(const std::string& name, glm::vec3& value, float min, float max,
                                               float step)
    {
        ImGui::PushID(name.c_str());
        ImGui::Columns(2);
        ImGui::SetColumnWidth(0, 100.0f);
        ImGui::PushStyleVar(ImGuiStyleVar_ItemSpacing, ImVec2{0, 5});
        ImGui::AlignTextToFramePadding();
        ImGui::Text(name.c_str());
        ImGui::PopStyleVar();
        ImGui::NextColumn();

        ImGui::PushMultiItemsWidths(3, ImGui::CalcItemWidth());
        ImGui::PushStyleVar(ImGuiStyleVar_ItemSpacing, ImVec2{0, 5});

        float lineHeight = ImGui::GetTextLineHeightWithSpacing();
        ImVec2 buttonSize = {lineHeight + 3.0f, lineHeight};

        ImGui::PushStyleColor(ImGuiCol_Button, ImVec4{0.8f, 0.1f, 0.15f, 1.0f});
        ImGui::PushStyleColor(ImGuiCol_ButtonHovered, ImVec4{0.9f, 0.2f, 0.2f, 1.0f});
        ImGui::PushStyleColor(ImGuiCol_ButtonActive, ImVec4{0.8f, 0.1f, 0.15f, 1.0f});
        if (ImGui::Button("X", buttonSize))
            value.x = 0.0f;
        ImGui::PopStyleColor(3);

        ImGui::SameLine();
        ImGui::DragFloat("##X", &value.x, step, min, max, "%.2f");
        ImGui::PopItemWidth();
        ImGui::SameLine();

        ImGui::PushStyleColor(ImGuiCol_Button, ImVec4{0.2f, 0.7f, 0.2f, 1.0f});
        ImGui::PushStyleColor(ImGuiCol_ButtonHovered, ImVec4{0.3f, 0.8f, 0.3f, 1.0f});
        ImGui::PushStyleColor(ImGuiCol_ButtonActive, ImVec4{0.2f, 0.7f, 0.2f, 1.0f});
        if (ImGui::Button("Y", buttonSize))
            value.y = 0.0f;
        ImGui::PopStyleColor(3);

        ImGui::SameLine();
        ImGui::DragFloat("##Y", &value.y, step, min, max, "%.2f");
        ImGui::PopItemWidth();
        ImGui::SameLine();

        ImGui::PushStyleColor(ImGuiCol_Button, ImVec4{0.1f, 0.25f, 0.8f, 1.0f});
        ImGui::PushStyleColor(ImGuiCol_ButtonHovered, ImVec4{0.2f, 0.35f, 0.9f, 1.0f});
        ImGui::PushStyleColor(ImGuiCol_ButtonActive, ImVec4{0.1f, 0.25f, 0.8f, 1.0f});
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
        if (ImGui::ColorEdit3(id.c_str(), glm::value_ptr(value), ImGuiColorEditFlags_NoInputs))
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
        ImGui::PushStyleVar(ImGuiStyleVar_ItemSpacing, ImVec2{0, 5});
        ImGui::AlignTextToFramePadding();
        ImGui::Text(name.c_str());
        ImGui::PopStyleVar();
        ImGui::NextColumn();
        
        const std::string id = "##" + name;
        if (ImGui::ColorEdit4(id.c_str(), glm::value_ptr(value), ImGuiColorEditFlags_NoInputs))
        {
            modified = true;
        }
        
        ImGui::Columns(1);
        ImGui::PopID();

        return modified;
    }
}
