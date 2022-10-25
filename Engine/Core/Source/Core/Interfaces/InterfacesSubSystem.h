#pragma once
#include "Core/Subsystems/SubSystem.h"

#include "Renderer/Interfaces/InterfaceImplementation.h"

namespace Retro
{
	class InterfacesSubSystem : public SubSystem
	{
	public:
		/* Constructor & Destructor */
		InterfacesSubSystem();
		~InterfacesSubSystem() override;

		/* Methods */
		void OnSubSystemStart() override;
		void OnSubSystemStop() override;

		void InitializeImGui() const;
		void TerminateImGui() const;

		/* Instantiate */
		static Scope<InterfacesSubSystem> Create();
	private:
		Scope<Renderer::InterfaceImplementation> m_InterfaceImplementation;
	};
}
