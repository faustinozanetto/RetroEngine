#include "pch.h"

#include "RetroApplication.h"
#include "Logger/Logger.h"

namespace Retro {
	RetroApplication* RetroApplication::s_Instance = nullptr;

	RetroApplication::RetroApplication(const FRetroApplicationSpecification& applicationSpecification) {
		// Update variables.
		s_Instance = this;
		m_ApplicationSpecification = applicationSpecification;
		// Create window.
		//FWindowSpecification windowSpecification = {};
		//m_Window = Window::Create(windowSpecification);
	}

	RetroApplication::~RetroApplication() = default;

	void RetroApplication::RunApplication()
	{
		while (true) {}
	}
}