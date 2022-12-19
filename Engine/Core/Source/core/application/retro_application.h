#pragma once

#include "core/base.h"
#include "core/interfaces/interface_subsystem.h"
#include "core/scene/scene_manager.h"
#include "renderer/window/window.h"

int main(int argc, char** argv);

namespace retro
{
	struct application_command_args {
		char** args;

		const char* operator[](int index) {
			return args[index];
		}
	};
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
		retro_application(const retro_application_specification& retro_application_specification);
		virtual ~retro_application();

		/* Methods */
		void run_application() const;

		/* Getters */
		const unique<renderer::window>& get_window() const;

		static retro_application& get_application();

	private:
		retro_application_specification m_application_specification;
		unique<renderer::window> m_window;
		unique<interface_subsystem> m_interfaces_subsystem;
		static retro_application* s_instance;
	};

	retro_application* create_retro_application(application_command_args command_args);
}
