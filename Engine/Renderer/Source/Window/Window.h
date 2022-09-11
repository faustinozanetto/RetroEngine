#pragma once

#include "EngineCore.h"

namespace Retro::Renderer
{
    struct FWindowSpecification {
        std::string windowTitle;
        uint32_t width;
        uint32_t height;

        FWindowSpecification(const std::string& windowTitle = "Retro Engine", uint32_t width = 1920,
            uint32_t height = 1080) : windowTitle(windowTitle), width(width), height(height) {}
    };

    class Window
    {
    public:
        /* Destructor */
        virtual ~Window() = default;
        
        /* Getters */
        const FWindowSpecification& GetWindowSpecification();

        /* Instantiate */
        static Core::Scope<Window> Create(const FWindowSpecification& specification = FWindowSpecification());
    protected:
        FWindowSpecification m_WindowSpecification;
    };
}
