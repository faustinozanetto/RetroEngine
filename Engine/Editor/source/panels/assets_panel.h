#pragma once

#include "editor_panel.h"

namespace retro::editor
{
    class assets_panel : public editor_panel
    {
    public:
        assets_panel();
        ~assets_panel() override;

        void on_update_panel(float time) override;
        void on_render_panel() override;
    };
}
