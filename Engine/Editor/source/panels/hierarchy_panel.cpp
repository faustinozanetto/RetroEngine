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
        if (active_scene)
        {
            active_scene->get_actor_registry().each([&](entt::entity actor_handle)
            {
                actor actor{actor_handle, active_scene.get()};
                render_actor_details(actor);
            });
            if (ImGui::IsMouseDown(0) && ImGui::IsWindowHovered())
                editor_main_interface::s_selected_actor = {};
        }
        ImGui::End();
    }

    void hierarchy_panel::render_actor_details(actor actor)
    {
        auto& name_component =actor.get_component<retro::name_component>();
        ImGuiTreeNodeFlags flags = ((editor_main_interface::s_selected_actor == actor.get_actor_handle()) ? ImGuiTreeNodeFlags_Selected : 0) | ImGuiTreeNodeFlags_OpenOnArrow;
        flags |= ImGuiTreeNodeFlags_SpanAvailWidth;
        bool opened = ImGui::TreeNodeEx((void*)(uint64_t)(uint32_t)actor.get_actor_handle(), flags, name_component.name.c_str());
        if (ImGui::IsItemClicked())
        {
            editor_main_interface::s_selected_actor = actor.get_actor_handle();
        }
        if (opened)
        {
            ImGuiTreeNodeFlags flags = ImGuiTreeNodeFlags_OpenOnArrow | ImGuiTreeNodeFlags_SpanAvailWidth;
            bool opened = ImGui::TreeNodeEx((void*)9817239, flags, name_component.name.c_str());
            if (opened)
                ImGui::TreePop();
            ImGui::TreePop();
        }
    }
}
