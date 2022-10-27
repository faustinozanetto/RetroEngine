#pragma once

#include "core/base.h"
#include "renderer/renderer/graphics_object.h"
#include "renderer/buffers/vbo/vertex_object_buffer.h"

namespace retro::renderer
{
	class uniform_buffer : public graphics_object
	{
	public:
		/* Destructor */
		~uniform_buffer() override;

		/* Methods */
		virtual void set_data(const void* data, uint32_t size, uint32_t offset = 0) = 0;
		virtual void set_layout(const vertex_buffer_layout& layout, uint32_t block, uint32_t count = 1) = 0;

		/* Instantiate */
		static shared<uniform_buffer> create(uint32_t size, uint32_t binding);
	};
}
