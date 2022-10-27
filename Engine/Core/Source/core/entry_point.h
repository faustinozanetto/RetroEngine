#pragma once

#include "core/application/retro_application.h"

#ifdef RETRO_PLATFORM_WINDOWS
extern retro::retro_application* retro::create_retro_application();

int main(int argc, char **argv)
{
	const auto application = retro::create_retro_application();
	application->run_application();
	delete application;
}
#endif
