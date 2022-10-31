#include "editor_main_interface.h"

#include "imgui.h"
#include "core/application/retro_application.h"
#include "panels/actor_details_panel.h"
#include "panels/camera_panel.h"
#include "panels/hierarchy_panel.h"
#include "panels/renderer_panel.h"
#include "panels/statistics_panel.h"
#include "renderer/renderer/renderer.h"
#include "renderer/renderer/scene_renderer.h"

namespace retro::editor
{
    entt::entity editor_main_interface::s_selected_actor = entt::null;
    uint32_t editor_main_interface::s_render_target = 0;

    editor_main_interface::editor_main_interface(const std::string& interface_layer_name) : interface_layer(
        interface_layer_name)
    {
    }

    editor_main_interface::~editor_main_interface()
    {
    }

    void editor_main_interface::on_layer_registered()
    {
        auto statistics_panel = create_shared<editor::statistics_panel>();
        m_panels.emplace_back(statistics_panel);
        auto hierarchy_panel = create_shared<editor::hierarchy_panel>();
        m_panels.emplace_back(hierarchy_panel);
        auto actor_details_panel = create_shared<editor::actor_details_panel>();
        m_panels.emplace_back(actor_details_panel);
        auto camera_panel = create_shared<editor::camera_panel>();
        m_panels.emplace_back(camera_panel);
        auto renderer_panel = create_shared<editor::renderer_panel>();
        m_panels.emplace_back(renderer_panel);

        s_render_target = renderer::scene_renderer::get_final_texture();
    }

    void editor_main_interface::on_layer_unregistered()
    {
    }

    void editor_main_interface::on_layer_updated()
    {
        const float time = renderer::renderer::get_time();
        for (const auto& panel : m_panels)
        {
            panel->on_update_panel(time);
        }
    }

    void editor_main_interface::on_interface_renderer()
    {
        start_dockspace();
        render_viewport();
        for (const auto& panel : m_panels)
        {
            panel->on_render_panel();
        }
        end_dockspace();
    }

    void editor_main_interface::start_dockspace()
    {
        bool open = true;
        const ImGuiDockNodeFlags dockspaceFlags = ImGuiDockNodeFlags_None;
        ImGuiWindowFlags windowFlags = ImGuiWindowFlags_MenuBar | ImGuiWindowFlags_NoDocking;
        const ImGuiViewport* viewport = ImGui::GetMainViewport();
        ImGui::SetNextWindowPos(viewport->Pos);
        ImGui::SetNextWindowSize(viewport->Size);
        ImGui::SetNextWindowViewport(viewport->ID);
        windowFlags |= ImGuiWindowFlags_NoTitleBar;
        windowFlags |= ImGuiWindowFlags_NoBringToFrontOnFocus | ImGuiWindowFlags_NoNavFocus;

        ImGui::Begin("DockSpace", &open, windowFlags);

        // DockSpace
        const ImGuiIO& io = ImGui::GetIO();
        ImGuiStyle& style = ImGui::GetStyle();
        const float minWinSizeX = style.WindowMinSize.x;
        if (io.ConfigFlags & ImGuiConfigFlags_DockingEnable)
        {
            const ImGuiID dockspace_id = ImGui::GetID("EditorDockspace");
            ImGui::DockSpace(dockspace_id, ImVec2(0.0f, 0.0f), dockspaceFlags);
        }
        style.WindowMinSize.x = minWinSizeX;
    }

    void editor_main_interface::end_dockspace()
    {
        ImGui::End();
    }

    void editor_main_interface::render_viewport()
    {
        ImGui::Begin("Viewport");
        const ImVec2 viewportPanelSize = ImGui::GetContentRegionAvail();
        // Draw viewport
        ImGui::Image(
            reinterpret_cast<ImTextureID>(s_render_target),
            ImVec2{viewportPanelSize.x, viewportPanelSize.y}, ImVec2{0, 1},
            ImVec2{1, 0});
        ImGui::End();
    }
}
