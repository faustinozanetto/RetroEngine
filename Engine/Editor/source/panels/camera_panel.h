#pragma once

#include "editor_panel.h"

namespace retro::editor
{
    class camera_panel : public editor_panel
    {
    public:
        camera_panel();
        ~camera_panel() override;

        void on_update_panel(float time) override;
        void on_render_panel() override;

    private:
        float m_camera_fov = 50.0f;
        float m_near_plane = 0.1f;
        float m_far_plane = 1000.0f;
    };
}
