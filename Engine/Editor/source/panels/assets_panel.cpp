#include "assets_panel.h"

#include "imgui.h"

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
        ImGui::Begin("Assets");

        
        ImGui::End();
    }
}
