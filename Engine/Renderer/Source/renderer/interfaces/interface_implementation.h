#pragma once

#include "core/base.h"

namespace retro::renderer
{
	class interface_implementation
	{
	public:
		/*  Destructor */
		virtual ~interface_implementation() = default;

		/* Methods */
		virtual void initialize_interface() = 0;
		virtual void shutdown_interface() = 0;
		virtual void begin_interface_render() = 0;
		virtual void end_interface_render() = 0;

		/* Instantiate */
		static unique<interface_implementation> create();

	private:
	};
}
