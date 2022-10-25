#include "pch.h"

#include "InterfacesSubSystem.h"

#include "Core/Application/RetroApplication.h"

namespace Retro
{
	InterfacesSubSystem::InterfacesSubSystem() : SubSystem("InterfacesSubSystem")
	{
		m_InterfaceImplementation = Renderer::InterfaceImplementation::Create();
	}

	InterfacesSubSystem::~InterfacesSubSystem()
	{
	}

	void InterfacesSubSystem::OnSubSystemStart()
	{
		m_InterfaceImplementation->InitializeInterface();
	}

	void InterfacesSubSystem::OnSubSystemStop()
	{
		m_InterfaceImplementation->ShutdownInterface();
	}

	void InterfacesSubSystem::InitializeImGui() const
	{
		m_InterfaceImplementation->BeginInterfaceRender();
	}

	void InterfacesSubSystem::TerminateImGui() const
	{
		m_InterfaceImplementation->EndInterfaceRender();
	}

	Unique<InterfacesSubSystem> InterfacesSubSystem::Create()
	{
		return CreateUnique<InterfacesSubSystem>();
	}
}
