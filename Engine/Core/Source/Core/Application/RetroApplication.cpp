#include "pch.h"
#include "RetroApplication.h"

namespace Retro::Core {
	RetroApplication* RetroApplication::s_Instance = nullptr;

	RetroApplication::RetroApplication(const FRetroApplicationSpecification& applicationSpecification) {
		// Update variables.
		s_Instance = this;
		m_ApplicationSpecification = applicationSpecification;
	}

	RetroApplication::~RetroApplication() = default;
}