#include "pch.h"

#include "retro_application.h"

#include "core/assets/assets_manager.h"
#include "core/interfaces/interfaces_layer_manager.h"
#include "core/interfaces/interface_layer.h"
#include "core/layers/layer_manager.h"
#include "renderer/renderer/renderer.h"

namespace retro
{
	retro_application* retro_application::s_instance = nullptr;

	retro_application::retro_application() : retro_application({ "Retro Application" })
	{
	}

	retro_application::retro_application(const retro_application_specification& retro_application_specification)
	{
		// Set working directory.
		std::filesystem::current_path(retro_application_specification.working_directory);
		// Update variables.
		s_instance = this;
		m_application_specification = retro_application_specification;
		// Initialize Logger.
		logger::initialize();
		logger::line();
		// Initialize managers.
		assets_manager::initialize();
		layer_manager::initialize();
		interfaces_layer_manager::initialize();
		scene_manager::initialize();
		// Create window.
		const renderer::window_specification windowSpecification = renderer::window_specification(
			"Retro Engine", 1920, 1080, true);
		m_window = renderer::window::create(windowSpecification);
		// Initialize Renderer
		renderer::renderer::initialize(renderer::renderer_api_type::open_gl, *m_window.get());
		// Initialize Interfaces subsystem
		m_interfaces_subsystem = interface_subsystem::create();
		m_interfaces_subsystem->on_sub_system_start();
	}

	retro_application::~retro_application()
	{
		layer_manager::get().shutdown();

		assets_manager::release();
		layer_manager::release();
		interfaces_layer_manager::release();
		scene_manager::release();
	};

	void retro_application::run_application() const
	{
		while (!renderer::renderer::should_close())
		{
			renderer::renderer::begin();
			m_interfaces_subsystem->initialize_imgui();
			// Main Render Loop.
			{
				// Update layers.
				for (auto it = layer_manager::get().get_layer_stack().begin(); it <
					layer_manager::get().get_layer_stack().end();
					++it)
				{
					it->get()->on_layer_updated();
				}
			}

			// Main Interfaces Loop.
			{
				// Update interface layer.
				for (auto it = interfaces_layer_manager::get().get_interface_layer_stack().begin(); it <
					interfaces_layer_manager::get().get_interface_layer_stack().end();
					++it)
				{
					const auto interfaceLayer = dynamic_cast<interface_layer*>(it->get());
					interfaceLayer->on_layer_updated();
					interfaceLayer->on_interface_renderer();
				}
			}
			m_interfaces_subsystem->terminate_imgui();

			renderer::renderer::end();
		}
	}

	const unique<renderer::window>& retro_application::get_window() const
	{
		return m_window;
	}

	retro_application& retro_application::get_application()
	{
		return *s_instance;
	}
}