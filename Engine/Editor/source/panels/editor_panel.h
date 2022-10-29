#pragma once

namespace retro::editor
{
    class editor_panel
    {
    public:
        virtual ~editor_panel() = default;

        virtual void on_update_panel(float time) = 0;
        virtual void on_render_panel() = 0;
    };
}