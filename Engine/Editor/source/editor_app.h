#pragma once

#include "core/application/retro_application.h"

namespace retro::editor
{
    class editor_app : public retro_application
    {
    public:
        editor_app();
        ~editor_app() override;
    };
}
