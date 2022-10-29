#include "editor_interface_utils.h"

#include <glm/gtc/type_ptr.hpp>

#include "imgui.h"

namespace retro::editor
{
    bool editor_interface_utils::draw_property(const std::string& name, glm::vec3& value, float min, float max,
                                               float step)
    {
        bool modified = false;
        ImGui::TextUnformatted(name.c_str());
        ImGui::NextColumn();
        ImGui::PushItemWidth(-1);

        const std::string id = "##" + name;
        if (ImGui::DragFloat3(id.c_str(), glm::value_ptr(value), step, min, max))
        {
            modified = true;
        }
        
        ImGui::PopItemWidth();
        ImGui::NextColumn();

        return modified;
    }
}
