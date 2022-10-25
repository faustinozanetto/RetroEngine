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
		void RegisterLayer(const Ref<Layer>& layer);
		const std::deque<Ref<Layer>>& GetLayerStack() const;

		/* Instantiate */
		static Scope<LayerManager> Create(const std::string& layerManagerName);
	private:
		std::deque<Ref<Layer>> m_LayerStack;
	};
}
