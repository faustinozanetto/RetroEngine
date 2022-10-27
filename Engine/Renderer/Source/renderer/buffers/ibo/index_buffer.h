#pragma once

#include "core/base.h"
#include "renderer/renderer/graphics_object.h"

namespace Retro::Renderer
{
	class index_buffer : public graphics_object
	{
	public:
		/* Destructor */
		~index_buffer() override;

		/* Methods */
		virtual uint32_t get_size() const = 0;

		/* Instantiate */
		static shared<index_buffer> create(uint32_t* data, uint32_t size);
	};
}
