#include "hierarchy_panel.h"

#include "imgui.h"
#include "../editor_main_interface.h"
#include "core/application/retro_application.h"
#include "core/scene/actor.h"
#include "core/scene/components.h"

namespace retro::editor
{
    hierarchy_panel::hierarchy_panel()
    {
    }

    hierarchy_panel::~hierarchy_panel()
    {
    }

    void hierarchy_panel::on_update_panel(float time)
    {
    }

    void hierarchy_panel::on_render_panel()
    {
        const shared<scene>& active_scene = retro_application::get_application().get_scene_manager()->
            get_active_scene();
        ImGui::Begin("Hierarchy");
        if (ImGui::TreeNode("Scene Actors"))
        {
            for (auto& actor : active_scene->get_actors())
            {
                if (active_scene->get_actor_registry().has<name_component>(actor->get_actor_handle()))
                {
                    auto& name_component = active_scene->get_actor_registry().get<retro::name_component>(
                        actor->get_actor_handle());
                    if (ImGui::TreeNode(reinterpret_cast<void*>(static_cast<intptr_t>(actor->get_actor_handle())),
                                        "Actor %s",
                                        name_component.name.c_str()))
                    {
                        editor_main_interface::s_selected_actor = actor->get_actor_handle();
                        ImGui::TreePop();
                    }
                }
            }
            ImGui::TreePop();
        }
        ImGui::End();
    }
}
