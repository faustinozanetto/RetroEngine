#pragma once

#include "core/base.h"
#include "editor_panel.h"
#include "core/scene/actor.h"

namespace retro::editor
{
    class hierarchy_panel : public editor_panel
    {
    public:
        hierarchy_panel();
        ~hierarchy_panel() override;

        void on_update_panel(float time) override;
        void on_render_panel() override;

    private:
        void render_actor_details(actor actor);
    };
}
