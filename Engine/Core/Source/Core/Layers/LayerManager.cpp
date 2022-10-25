#include "pch.h"
#include "LayerManager.h"

namespace Retro
{
	LayerManager::LayerManager(const std::string& layerManagerName): Manager(layerManagerName)
	{
	}

	LayerManager::~LayerManager()
	{
	}

	void LayerManager::Shutdown()
	{
		// Delete all layers
		while (!m_LayerStack.empty())
		{
			// Detach and delete.
			const Shared<Layer>& layer = m_LayerStack.front();
			layer->OnLayerUnregistered();
			m_LayerStack.pop_front();
		}
		m_LayerStack.clear();
	}

	void LayerManager::RegisterLayer(const Shared<Layer>& layer)
	{
		m_LayerStack.push_front(layer);
		layer->OnLayerRegistered();
		Logger::Info("LayerManager::RegisterLayer | Registered layer: " + layer->GetLayerName());
	}

	const std::deque<Shared<Layer>>& LayerManager::GetLayerStack() const
	{
		return m_LayerStack;
	}

	Unique<LayerManager> LayerManager::Create(const std::string& layerManagerName)
	{
		return CreateUnique<LayerManager>(layerManagerName);
	}
}
