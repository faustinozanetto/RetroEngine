#pragma once

#include "Core/Base.h"
#include "Core/Interfaces/InterfacesSubSystem.h"
#include "Core/Layers/LayerManager.h"
#include "Core/Assets/AssetsManager.h"
#include "Renderer/Window/Window.h"

int main(int argc, char** argv);

namespace Retro
{
	struct FRetroApplicationSpecification
	{
		std::string name;

		FRetroApplicationSpecification(const std::string& name = "Retro Application") : name(name)
		{
		}
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
		const Unique<Renderer::Window>& GetWindow() const;
		const Unique<LayerManager>& GetLayersManager() const;
		const Unique<LayerManager>& GetInterfaceLayersManager() const;
		const Unique<AssetsManager>& GetAssetsManager() const;

		static RetroApplication& GetApplication();

	private:
		FRetroApplicationSpecification m_ApplicationSpecification;
		Unique<Renderer::Window> m_Window;
		Unique<AssetsManager> m_AssetsManager;
		Unique<LayerManager> m_LayersManager;
		Unique<LayerManager> m_InterfaceLayersManager;
		Unique<InterfacesSubSystem> m_InterfacesSubSystem;
		static RetroApplication* s_Instance;
	};

	RetroApplication* CreateRetroApplication();
}
