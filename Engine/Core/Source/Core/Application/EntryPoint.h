#pragma once

#ifdef RETRO_PLATFORM_WINDOWS

extern Retro::Core::Application* Retro::Core::CreateMechaApp();

extern "C" {
    __declspec(dllexport) uint32_t NvOptimusEnablement = 0x00000001;
}

int main(int argc, char** argv)
{
    //MECHA_DEBUG("Initializing MechaEngine...");
    // App creation and running.
    const auto application = Retro::Core::CreateRetroApplication();
    application->RunApp();
    delete application;
}

#endif
