#include "pch.h"

#include "interfaces_layer_manager.h"

namespace retro
{
	interfaces_layer_manager::interfaces_layer_manager()
	{
	}

	interfaces_layer_manager::~interfaces_layer_manager()
	{
	}

	void interfaces_layer_manager::shutdown()
	{
		// Delete all layers
		while (!m_interfaces_layer_stack.empty())
		{
			// Detach and delete.
			const shared<layer>& layer = m_interfaces_layer_stack.front();
			layer->on_layer_unregistered();
			m_interfaces_layer_stack.pop_front();
		}
		m_interfaces_layer_stack.clear();
	}

	void interfaces_layer_manager::register_interface_layer(const shared<interface_layer>& layer)
	{
		m_interfaces_layer_stack.push_front(layer);
		layer->on_layer_registered();
		logger::info("interfaces_layer_manager::register_interface_layer | Registered layer: " + layer->get_layer_name());
	}

	std::deque<shared<interface_layer>>& interfaces_layer_manager::get_interface_layer_stack()
	{
		return m_interfaces_layer_stack;
	}
}