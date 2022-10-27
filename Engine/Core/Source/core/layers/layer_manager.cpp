#include "pch.h"
#include "layer_manager.h"

namespace Retro
{
	layer_manager::layer_manager(const std::string &layer_manager_name) : manager(layer_manager_name)
	{
	}

	layer_manager::~layer_manager()
	{
	}

	void layer_manager::shutdown()
	{
		// Delete all layers
		while (!m_layer_stack.empty())
		{
			// Detach and delete.
			const shared<layer> &layer = m_layer_stack.front();
			layer->on_layer_unregistered();
			m_layer_stack.pop_front();
		}
		m_layer_stack.clear();
	}

	void layer_manager::register_layer(const shared<layer> &layer)
	{
		m_layer_stack.push_front(layer);
		layer->on_layer_registered();
		Logger::Info("LayerManager::RegisterLayer | Registered layer: " + layer->get_layer_name());
	}

	const std::deque<Shared<Layer>> &layer_manager::get_layer_stack() const
	{
		return m_layer_stack;
	}

	unique<layer_manager> layer_manager::create(const std::string &layer_manager_name)
	{
		return create_unique<layer_manager>(layer_manager_name);
	}
}
