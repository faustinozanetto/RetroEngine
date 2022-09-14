#pragma once

#include "Core/Base.h"

namespace Retro::Renderer
{
    struct FWindowSpecification {
        std::string windowTitle;
        uint32_t width;
        uint32_t height;
        bool vSync;

        FWindowSpecification(const std::string& windowTitle = "Retro Engine", uint32_t width = 1920,
            uint32_t height = 1080, bool vSync = false) : windowTitle(windowTitle), width(width), height(height), vSync(vSync) {}
    };

    class Window
    {
    public:
        /* Destructor */
        virtual ~Window() = default;

        /* Methods */
        virtual void SetEnableVSync(bool useVSync) = 0;
        virtual void SetupWindowCallbacks() = 0;

        /* Getters */
        const FWindowSpecification& GetWindowSpecification();
        virtual void* GetNativeWindow() const = 0;
        bool IsVSyncEnabled() const;

        /* Instantiate */
        static Scope<Window> Create(const FWindowSpecification& specification = FWindowSpecification());
    protected:
        FWindowSpecification m_WindowSpecification;
    };
}
