#include "pch.h"

#include "layer.h"

namespace retro
{
	layer::layer(const std::string &layer_name)
	{
		m_layer_name = layer_name;
	}

	const std::string &layer::get_layer_name() const
	{
		return m_layer_name;
	}
}
