#include "pch.h"

#include "interface_subsystem.h"

namespace Retro
{
	interface_subsystem::interface_subsystem() : sub_system("interface_subsystem")
	{
		m_interface_implementation = Renderer::InterfaceImplementation::create();
	}

	interface_subsystem::~interface_subsystem()
	{
	}

	void interface_subsystem::on_sub_system_start()
	{
		m_interface_implementation->InitializeInterface();
	}

	void interface_subsystem::on_sub_system_stop()
	{
		m_interface_implementation->ShutdownInterface();
	}

	void interface_subsystem::initialize_imgui() const
	{
		m_interface_implementation->BeginInterfaceRender();
	}

	void interface_subsystem::terminate_imgui() const
	{
		m_interface_implementation->EndInterfaceRender();
	}

	unique<interface_subsystem> interface_subsystem::create()
	{
		return create_unique<interface_subsystem>();
	}
}
