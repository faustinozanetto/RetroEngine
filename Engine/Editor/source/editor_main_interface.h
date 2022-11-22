#pragma once

#include <entt/entt.hpp>

#include "core/base.h"
#include "core/interfaces/interface_layer.h"
#include "panels/editor_panel.h"

namespace retro::editor
{
    class editor_main_interface : public interface_layer
    {
    public:
        editor_main_interface(const std::string& interface_layer_name);
        ~editor_main_interface() override;

        void on_layer_registered() override;
        void on_layer_unregistered() override;
        void on_layer_updated() override;
        void on_interface_renderer() override;


        static entt::entity s_selected_actor;
        static uint32_t s_render_target;

    private:
        void start_dockspace();
        void end_dockspace();
        void render_viewport();

        std::vector<shared<editor_panel>> m_panels;
    };
}
