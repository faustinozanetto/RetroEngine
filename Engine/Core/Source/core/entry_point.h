#pragma once

#include "core/application/retro_application.h"

#ifdef RETRO_PLATFORM_WINDOWS
extern retro::retro_application* retro::create_retro_application(application_command_args command_args);

int main(int argc, char** argv)
{
	const auto application = retro::create_retro_application({ argv });
	application->run_application();
	delete application;
}
#endif
