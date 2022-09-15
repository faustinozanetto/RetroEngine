#pragma once
#include "Core/Subsystems/SubSystem.h"

namespace Retro
{
    class InterfacesSubSystem : public SubSystem
    {
    public:
        /* Constructor & Destructor */
        InterfacesSubSystem();
        ~InterfacesSubSystem() override;

        /* Methods */
        void OnSubSystemStart() override;
        void OnSubSystemStop() override;

        void InitializeImGui();
        void TerminateImGui();

        /* Instantiate */
        static Scope<InterfacesSubSystem> Create();
    };
}
