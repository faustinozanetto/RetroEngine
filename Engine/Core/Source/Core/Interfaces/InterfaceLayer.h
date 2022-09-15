#pragma once
#include "Core/Layers/Layer.h"

namespace Retro
{
    class InterfaceLayer : public Layer
    {
    public:
        /* Constructor & Destructor */
        InterfaceLayer(const std::string& interfaceLayerName);
        ~InterfaceLayer() override = default;

        /* Methods */
        virtual void OnInterfaceRenderer() = 0;
    };
}
