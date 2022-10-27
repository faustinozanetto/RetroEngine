#pragma once

#include "core/base.h"
#include "core/interfaces/interface_subsystem.h"
#include "core/layers/layer_manager.h"
#include "core/assets/assets_manager.h"
#include "renderer/window/window.h"

int main(int argc, char **argv);

namespace retro
{
	struct retro_application_specification
	{
		std::string name;

		retro_application_specification(std::string name = "Retro Application") : name(std::move(name))
		{
		}
	};

	class retro_application
	{
	public:
		/* Constructors & Destructors */
		retro_application();
		retro_application(const retro_application_specification &retro_application_specification);
		virtual ~retro_application();

		/* Methods */
		void run_application() const;

		/* Getters */
		const unique<renderer::window> &get_window() const;
		const unique<layer_manager> &get_layers_manager() const;
		const unique<layer_manager> &get_interfaces_layer_manager() const;
		const unique<assets_manager> &get_assets_manager() const;

		static retro_application &get_application();

	private:
		retro_application_specification m_application_specification;
		unique<renderer::window> m_window;
		unique<assets_manager> m_assets_manager;
		unique<layer_manager> m_layers_manager;
		unique<layer_manager> m_interface_layers_manager;
		unique<interface_subsystem> m_interfaces_subsystem;
		static retro_application *s_instance;
	};

	retro_application * create_retro_application();
}
