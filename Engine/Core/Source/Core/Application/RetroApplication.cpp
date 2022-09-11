#include "pch.h"

#include "RetroApplication.h"
#include "Logger/Logger.h"

namespace Retro::Core {
	RetroApplication* RetroApplication::s_Instance = nullptr;

	RetroApplication::RetroApplication(const FRetroApplicationSpecification& applicationSpecification) {
		// Update variables.
		s_Instance = this;
		m_ApplicationSpecification = applicationSpecification;
	}

	RetroApplication::~RetroApplication() = default;

	void RetroApplication::RunApplication()
	{
		while (true) {}
	}
}