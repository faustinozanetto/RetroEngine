#pragma once

#include <stack>

#include "Layer.h"
#include "Core/Managers/Manager.h"

namespace Retro
{
	class LayerManager : public Manager
	{
	public:
		/* Constructor & Destructor */
		LayerManager(const std::string& layerManagerName);
		~LayerManager() override;

		/* Methods */
		void Shutdown() override;
		void RegisterLayer(const Shared<Layer>& layer);
		const std::deque<Shared<Layer>>& GetLayerStack() const;

		/* Instantiate */
		static Unique<LayerManager> Create(const std::string& layerManagerName);
	private:
		std::deque<Shared<Layer>> m_LayerStack;
	};
}
