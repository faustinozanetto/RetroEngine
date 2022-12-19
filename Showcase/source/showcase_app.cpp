#include "showcase_app.h"

#include "core/entry_point.h"
#include "showcase_layer.h"
#include "core/layers/layer_manager.h"

namespace retro
{
	showcase_app::showcase_app() : retro_application({ "showcase_app" })
	{
		layer_manager::get().register_layer(retro::create_shared<showcase_layer>("showcase_layer"));
	}

	showcase_app::~showcase_app()
	{
	}
}

retro::retro_application* retro::create_retro_application(application_command_args command_args)
{
	return new showcase_app();
}