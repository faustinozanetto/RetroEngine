#include "assets_panel.h"

#include "imgui.h"
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
                    std::string label = "UUID: " + std::to_string(asset.second->get_uuid().get()->get());
                    if (ImGui::TreeNodeEx(
                        reinterpret_cast<void*>(asset.first->get()),
                        flags, label.c_str()))
                    {
                        ImGui::TreePop();
                    }
                }
                ImGui::TreePop();
            }
        }

        ImGui::End();
    }
}
