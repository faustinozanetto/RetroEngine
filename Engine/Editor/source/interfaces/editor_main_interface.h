#pragma once

#include "core/base.h"
#include "core/interfaces/interface_layer.h"

namespace retro::editor
{
    class editor_main_interface : public interface_layer
    {
    public:
        editor_main_interface(const std::string &interface_layer_name);
        ~editor_main_interface() override;
        
        void on_layer_registered() override;
        void on_layer_unregistered() override;
        void on_layer_updated() override;
        void on_interface_renderer() override;
    };
}
