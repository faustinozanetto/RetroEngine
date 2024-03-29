﻿#include "pch.h"

#include "layer_manager.h"

namespace retro
{
	layer_manager::layer_manager()
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
			const shared<layer>& layer = m_layer_stack.front();
			layer->on_layer_unregistered();
			m_layer_stack.pop_front();
		}
		m_layer_stack.clear();
	}

	void layer_manager::register_layer(const shared<layer>& layer)
	{
		m_layer_stack.push_front(layer);
		layer->on_layer_registered();
		logger::info("layer_manager::register_layer | Registered layer: " + layer->get_layer_name());
	}

	std::deque<shared<layer>>& layer_manager::get_layer_stack()
	{
		return m_layer_stack;
	}
}