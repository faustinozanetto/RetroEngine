#pragma once

#include "Core/Base.h"
#include "Renderer/Buffers/VBO/VertexBuffer.h"
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
        void RunApplication() const;

        /* Getters */
        const Renderer::Window& GetWindow() const;
        
    private:
        FRetroApplicationSpecification m_ApplicationSpecification;
        Scope<Renderer::Window> m_Window;
        Ref<Renderer::VertexBuffer> m_VBO;
        static RetroApplication* s_Instance;
    };
    
    RetroApplication* CreateRetroApplication();
}