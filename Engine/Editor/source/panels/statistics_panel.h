#pragma once

#include "editor_panel.h"

namespace retro::editor
{
    class statistics_panel : public editor_panel
    {
    public:
        statistics_panel();
        ~statistics_panel() override;

        void on_update_panel(float time) override;
        void on_render_panel() override;
    };
}
