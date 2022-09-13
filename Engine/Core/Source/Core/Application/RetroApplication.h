#pragma once

#include "Core/Base.h"
#include "Renderer/Window/Window.h"

int main(int argc, char** argv);

namespace Retro
{
    struct FRetroApplicationSpecification {
        std::string name;

        FRetroApplicationSpecification(const std::string& name = "Retro Application") : name(name) {}
    };

    class RetroApplication
    {
    public:
        /* Constructors & Destructors */
        RetroApplication() = default;
        RetroApplication(const FRetroApplicationSpecification& applicationSpecification);
        virtual ~RetroApplication();

        /* Methods */
        void RunApplication();

        /* Getters */
        const Window& GetWindow() const;
        
    private:
        FRetroApplicationSpecification m_ApplicationSpecification;
        Scope<Window> m_Window;
        static RetroApplication* s_Instance;
    };
    
    RetroApplication* CreateRetroApplication();
}