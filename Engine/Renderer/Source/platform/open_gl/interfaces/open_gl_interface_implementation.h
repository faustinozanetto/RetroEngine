#pragma once

#include "renderer/interfaces/interface_implementation.h"

namespace retro::renderer
{
	class open_gl_interface_implementation : public interface_implementation
	{
	public:
		/* Constructor & Destructor */
		open_gl_interface_implementation();
		~open_gl_interface_implementation() override;

		/* Methods */
		void initialize_interface() override;
		void shutdown_interface() override;
		void begin_interface_render() override;
		void end_interface_render() override;
	};
}
