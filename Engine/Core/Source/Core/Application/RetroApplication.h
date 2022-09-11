#pragma once

#include "pch.h"

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
        
    private:
        FRetroApplicationSpecification m_ApplicationSpecification;
        static RetroApplication* s_Instance;

    };
    
    RetroApplication* CreateRetroApplication();
}