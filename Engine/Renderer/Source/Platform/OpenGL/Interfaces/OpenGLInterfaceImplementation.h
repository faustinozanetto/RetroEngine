#pragma once
#include "Renderer/Interfaces/InterfaceImplementation.h"

namespace Retro::Renderer
{
    class OpenGLInterfaceImplementation : public InterfaceImplementation
    {
    public:
        /* Constructor & Destructor */
        OpenGLInterfaceImplementation();
        ~OpenGLInterfaceImplementation() override;

        /* Methods */
        void InitializeInterface() override;
        void ShutdownInterface() override;
        void BeginInterfaceRender() override;
        void EndInterfaceRender() override;
    };
}
