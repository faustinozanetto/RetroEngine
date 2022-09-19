#pragma once

#include "Core/Base.h"
#include "Core/Interfaces/InterfacesSubSystem.h"
#include "Core/Layers/LayerManager.h"
#include "Renderer/Window/Window.h"

int main(int argc, char** argv);

namespace Retro
{
	struct FRetroApplicationSpecification {
		std::string name;

		FRetroApplicationSpecification(const std::string& name = "Retro Application") : name(name) {}
	};

	class RetroApplication
	{
	public:
		/* Constructors & Destructors */
		RetroApplication() = default;
		RetroApplication(const FRetroApplicationSpecification& applicationSpecification);
		virtual ~RetroApplication();

		/* Methods */
		void RunApplication() const;

		/* Getters */
		const Scope<Renderer::Window>& GetWindow() const;
		const Scope<LayerManager>& GetLayersManager() const;
		const Scope<LayerManager>& GetInterfaceLayersManager() const;

		static RetroApplication& GetApplication();

	private:
		FRetroApplicationSpecification m_ApplicationSpecification;
		Scope<Renderer::Window> m_Window;
		Scope<LayerManager> m_LayersManager;
		Scope<LayerManager> m_InterfaceLayersManager;
		Scope<InterfacesSubSystem> m_InterfacesSubSystem;
		static RetroApplication* s_Instance;
	};

	RetroApplication* CreateRetroApplication();
}