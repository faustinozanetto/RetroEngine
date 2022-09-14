#pragma once

namespace Retro
{
    class Layer
    {
    public:
        /* Constructor & Destructor */
        Layer(const std::string& layerName);
        virtual ~Layer() = default;

        /* Methods */
        virtual void OnLayerRegistered() = 0;
        virtual void OnLayerUnregistered() = 0;
        virtual void OnLayerUpdated() = 0;
        const std::string& GetLayerName() const;

        /* Instantiate */
    private:
        std::string m_LayerName;
    };
}
