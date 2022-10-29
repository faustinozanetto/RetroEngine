#include "editor_main_interface.h"

#include "imgui.h"

namespace retro::editor
{
    editor_main_interface::editor_main_interface(const std::string& interface_layer_name) : interface_layer(interface_layer_name)
    {
    }

    editor_main_interface::~editor_main_interface()
    {
    }

    void editor_main_interface::on_layer_registered()
    {
    }

    void editor_main_interface::on_layer_unregistered()
    {
    }

    void editor_main_interface::on_layer_updated()
    {
    }

    void editor_main_interface::on_interface_renderer()
    {
        ImGui::Begin("Editor");
        ImGui::Text("Test");
        ImGui::End();
    }
}
