#pragma once

#include "Core/Application/RetroApplication.h"
#include "Core/Base.h"

#ifdef RETRO_PLATFORM_WINDOWS
extern Retro::Core::RetroApplication* Retro::Core::CreateRetroApplication();

int main(int argc, char** argv)
{
    auto application = Retro::Core::CreateRetroApplication();
    application->RunApplication();
    delete application;
}
#endif