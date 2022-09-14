#include "pch.h"
#include "LayerManager.h"

namespace Retro
{
    LayerManager::LayerManager() : Manager("LayersManager")
    {
    }

    LayerManager::~LayerManager()
    {
        // Delete all layers
        while (!m_LayerStack.empty())
        {
            // Detach and delete.
            const Ref<Layer>& layer = m_LayerStack.front();
            layer->OnLayerUnregistered();
            m_LayerStack.pop_front();
        }
    }

    void LayerManager::RegisterLayer(const Ref<Layer>& layer)
    {
        m_LayerStack.push_front(layer);
        layer->OnLayerRegistered();
        Logger::Info("LayerManager::RegisterLayer | Registered layer: " + layer->GetLayerName());
    }

    const std::deque<Ref<Layer>>& LayerManager::GetLayerStack() const
    {
        return m_LayerStack;
    }

    Scope<LayerManager> LayerManager::Create()
    {
        return CreateScope<LayerManager>();
    }
}
