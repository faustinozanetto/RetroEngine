#include "pch.h"

#include "interface_subsystem.h"

namespace retro
{
	interface_subsystem::interface_subsystem() : sub_system("interface_subsystem")
	{
		m_interface_implementation = renderer::interface_implementation::create();
	}

	interface_subsystem::~interface_subsystem()
	{
	}

	void interface_subsystem::on_sub_system_start()
	{
		m_interface_implementation->initialize_interface();
	}

	void interface_subsystem::on_sub_system_stop()
	{
		m_interface_implementation->shutdown_interface();
	}

	void interface_subsystem::initialize_imgui() const
	{
		m_interface_implementation->begin_interface_render();
	}

	void interface_subsystem::terminate_imgui() const
	{
		m_interface_implementation->end_interface_render();
	}

	unique<interface_subsystem> interface_subsystem::create()
	{
		return create_unique<interface_subsystem>();
	}
}
