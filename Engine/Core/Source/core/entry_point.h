#pragma once

#include "core/application/retro_application.h"
#include "core/base.h"

#ifdef RETRO_PLATFORM_WINDOWS
extern Retro::retro_application *Retro::create_retro_application();

int main(int argc, char **argv)
{
	const auto application = Retro::create_retro_application();
	application->run_application();
	delete application;
}
#endif
