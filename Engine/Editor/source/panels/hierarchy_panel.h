#pragma once
#include "editor_panel.h"

namespace retro::editor
{
    class hierarchy_panel : public editor_panel
    {
    public:
        hierarchy_panel();
        ~hierarchy_panel() override;

        void on_update_panel(float time) override;
        void on_render_panel() override;
    };
}
