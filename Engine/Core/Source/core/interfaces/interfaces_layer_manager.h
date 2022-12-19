#pragma once

#include <stack>

#include "core/interfaces/interface_layer.h"
#include "core/utils/singleton.h"

namespace retro
{
	class interfaces_layer_manager : public singleton<interfaces_layer_manager>
	{
	public:
		interfaces_layer_manager();
		~interfaces_layer_manager();

		void register_interface_layer(const shared <interface_layer>& layer);
		void shutdown();

		std::deque<shared<interface_layer>>& get_interface_layer_stack();
	private:
		std::deque<shared<interface_layer>> m_interfaces_layer_stack;
	};
}
