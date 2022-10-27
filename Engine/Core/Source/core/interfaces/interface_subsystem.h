#pragma once

#include "core/subsystems/sub_system.h"
#include "renderer/interfaces/interface_implementation.h"

namespace retro
{
	class interface_subsystem : public sub_system
	{
	public:
		/* Constructor & Destructor */
		interface_subsystem();
		~interface_subsystem() override;

		/* Methods */
		void on_sub_system_start() override;
		void on_sub_system_stop() override;

		void initialize_imgui() const;
		void terminate_imgui() const;

		/* Instantiate */
		static unique<interface_subsystem> create();

	private:
		unique<renderer::interface_implementation> m_interface_implementation;
	};
}
