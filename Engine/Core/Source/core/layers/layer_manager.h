#pragma once

#include <stack>

#include "layer.h"
#include "core/utils/singleton.h"

namespace retro
{
	class layer_manager : public singleton<layer_manager>
	{
	public:
		layer_manager();
		~layer_manager();

		void register_layer(const shared<layer>& layer);
		void shutdown();

		std::deque<shared<layer>>& get_layer_stack();
	private:
		std::deque<shared<layer>> m_layer_stack;
	};
}
