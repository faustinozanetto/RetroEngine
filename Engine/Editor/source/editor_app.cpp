#include "editor_app.h"

#include "core/entry_point.h"
#include "editor_layer.h"
#include "editor_main_interface.h"
#include <iostream>

#include "core/interfaces/interfaces_layer_manager.h"
#include "core/layers/layer_manager.h"

namespace retro::editor
{
	editor_app::editor_app() : retro_application({ "Editor" })
	{
		layer_manager::get().register_layer(retro::create_shared<editor_layer>("EditorLayer"));
		interfaces_layer_manager::get().register_interface_layer(retro::create_shared<editor_main_interface>("EditorMainInterface"));
	}

	editor_app::~editor_app()
	{
	}
}

retro::retro_application* retro::create_retro_application(application_command_args command_args)
{
	return new editor::editor_app();
}