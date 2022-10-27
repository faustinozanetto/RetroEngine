#pragma once

#include "core/layers/layer.h"

namespace Retro
{
	class interface_layer : public layer
	{
	public:
		/* Constructor & Destructor */
		interface_layer(const std::string &interface_layer_name);
		~interface_layer() override = default;

		/* Methods */
		virtual void on_interface_renderer() = 0;
	};
}
