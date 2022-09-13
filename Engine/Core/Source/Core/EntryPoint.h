#pragma once

#include "Core/Application/RetroApplication.h"
#include "Core/Base.h"

#ifdef RETRO_PLATFORM_WINDOWS
extern Retro::RetroApplication* Retro::CreateRetroApplication();

inline int main(int argc, char** argv)
{
    const auto application = Retro::CreateRetroApplication();
    application->RunApplication();
    delete application;
}
#endif