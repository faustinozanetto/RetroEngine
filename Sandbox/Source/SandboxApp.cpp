#pragma once

#include "EngineCore.h"
#include "Core/EntryPoint.h"

class SandboxApplication : public Retro::RetroApplication
{
public:
	SandboxApplication() : RetroApplication({ "Sandbox" }) {

	}

	~SandboxApplication() {}
};

Retro::RetroApplication* Retro::CreateRetroApplication()
{
	return new SandboxApplication();
}