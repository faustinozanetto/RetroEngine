#pragma once
#include <glm/glm.hpp>

#include "editor_panel.h"

namespace retro::editor
{
    class actor_details_panel : public editor_panel
    {
    public:
        actor_details_panel();
        ~actor_details_panel() override;

        void on_update_panel(float time) override;
        void on_render_panel() override;
    private:
        glm::vec2 m_dir_light = {0.0f, 0.0f};
    };
}
