#include "editor_app.h"

#include "core/entry_point.h"
#include "editor_layer.h"
#include "interfaces/editor_main_interface.h"

namespace retro::editor
{
    editor_app::editor_app() : retro_application({"Editor"})
    {
        get_layers_manager()->register_layer(retro::create_shared<editor_layer>("EditorLayer"));
        get_interfaces_layer_manager()->register_layer(retro::create_shared<editor_main_interface>("EditorMainInterface"));
    }

    editor_app::~editor_app()
    {
    }
}

retro::retro_application* retro::create_retro_application()
{
    return new editor::editor_app();
}
