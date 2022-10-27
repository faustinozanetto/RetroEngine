#include "pch.h"

#include "retro_application.h"

#include "core/interfaces/interface_layer.h"
#include "Logger/Logger.h"
#include "Renderer/Renderer/Renderer.h"

namespace Retro
{
	retro_application *retro_application::s_instance = nullptr;

	retro_application::retro_application(const retro_application_specification &retro_application_specification)
	{
		std::filesystem::current_path("../");
		// Update variables.
		s_instance = this;
		m_application_specification = retro_application_specification;
		// Initialize Logger.
		Logger::Initialize();
		Logger::Line();
		// Create window.
		const auto windowSpecification = Renderer::FWindowSpecification(
				"Retro Engine", 1920, 1080, false);
		m_window = Renderer::Window::Create(windowSpecification);
		// Initialize Renderer
		Renderer::Renderer::Initialize(Renderer::RenderingAPIType::OpenGL, *m_window.get());
		// Initialize Layers and SubSystems.
		m_assets_manager = assets_mangaer::create();
		m_layers_manager = layer_manager::create("LayersManager");
		m_interface_layers_manager = layer_manager::create("InterfaceLayersManager");
		m_interfaces_subsystem = interface_subsystem::create();
		m_interfaces_subsystem->on_sub_system_start();
	}

	retro_application::~retro_application()
	{
		m_layers_manager->shutdown();
		m_assets_manager->shutdown();
	};

	void retro_application::run_application() const
	{
		while (!Renderer::Renderer::ShouldClose())
		{
			Renderer::Renderer::Begin();
			m_interfaces_subsystem->initialize_imgui();
			// Main Render Loop.
			{
				// Update layers.
				for (auto it = m_layers_manager->get_layer_stack().begin(); it <
																																		m_layers_manager->get_layer_stack().end();
						 ++it)
				{
					it->get()->on_layer_updated();
				}
			}

			// Main Interfaces Loop.
			{
				// Update interface layer.
				for (auto it = m_interface_layers_manager->get_layer_stack().begin(); it <
																																							m_interface_layers_manager->get_layer_stack().end();
						 ++it)
				{
					const auto interfaceLayer = dynamic_cast<interface_layer *>(it->get());
					interfaceLayer->on_layer_updated();
					interfaceLayer->on_interface_renderer();
				}
			}
			m_interfaces_subsystem->terminate_imgui();

			Renderer::Renderer::End();
		}
	}

	const unique<Renderer::Window> &retro_application::get_window() const
	{
		return m_window;
	}

	const unique<layer_manager> &retro_application::get_layers_manager() const
	{
		return m_layers_manager;
	}

	const unique<layer_manager> &retro_application::get_interfaces_layer_manager() const
	{
		return m_interface_layers_manager;
	}

	const unique<assets_mangaer> &retro_application::get_assets_manager() const
	{
		return m_assets_manager;
	}

	retro_application &retro_application::get_application()
	{
		return *s_instance;
	}
}
