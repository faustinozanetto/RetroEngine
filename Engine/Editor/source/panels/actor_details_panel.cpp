#include "actor_details_panel.h"

#include <glm/gtc/type_ptr.hpp>

#include "imgui.h"
#include "../editor_interface_utils.h"
#include "../editor_main_interface.h"
#include "core/application/retro_application.h"
#include "core/scene/components.h"
#include "core/scene/scene.h"
#include "core/scene/actor.h"

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
                const auto& name_component = active_scene->get_actor_registry().get<retro::name_component>(
                    editor_main_interface::s_selected_actor);
                ImGui::Text("Name: %s", name_component.name.c_str());
            }
            if (active_scene->get_actor_registry().has<transform_component>(editor_main_interface::s_selected_actor))
            {
                auto& transform_component = active_scene->get_actor_registry().get<retro::transform_component>(
                    editor_main_interface::s_selected_actor);
                editor_interface_utils::draw_property("Position", transform_component.position, -10.0f, 10.0f, 0.05f);
                editor_interface_utils::draw_property("Rotation", transform_component.rotation, -10.0f, 10.0f, 0.05f);
                editor_interface_utils::draw_property("Scale", transform_component.scale, 0.0f, 10.0f, 0.05f);
            }
        }
        else
        {
            ImGui::Text("No actor selected!");
        }
        ImGui::End();
    }
}
