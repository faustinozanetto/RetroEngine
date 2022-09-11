#pragma once

#include "EngineCore.h"
#include "Core/EntryPoint.h"

class SandboxApplication : public Retro::Core::RetroApplication
{
public:
	SandboxApplication() : RetroApplication({ "Sandbox" }) {

	}

	~SandboxApplication() {}
};

Retro::Core::RetroApplication* Retro::Core::CreateRetroApplication()
{
	return new SandboxApplication();
}