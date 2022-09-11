#pragma once

#include "pch.h"

#include "Core/Base.h"

int main(int argc, char** argv);

namespace Retro::Core
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
        
    private:
        FRetroApplicationSpecification m_ApplicationSpecification;
        static RetroApplication* s_Instance;

    };
    
    RetroApplication* CreateRetroApplication();
}