#pragma once

#include <stack>

#include "Layer.h"
#include "Core/Managers/Manager.h"

namespace Retro
{
	class layer_manager : public manager
	{
	public:
		/* Constructor & Destructor */
		layer_manager(const std::string &layer_manager_name);
		~layer_manager() override;

		/* Methods */
		void shutdown() override;
		void register_layer(const shared<layer> &layer);
		const std::deque<shared<layer>> &get_layer_stack() const;

		/* Instantiate */
		static unique<layer_manager> create(const std::string &layer_manager_name);

	private:
		std::deque<shared<layer>> m_layer_stack;
	};
}
