#pragma once

#include "core/base.h"
#include "renderer/buffers/ibo/index_buffer.h"
#include "renderer/buffers/vbo/vertex_object_buffer.h"
#include "renderer/renderer/graphics_object.h"

namespace retro::renderer
{
	class vertex_array_buffer : public graphics_object
	{
	public:
		/* Destructor */
		vertex_array_buffer();
		~vertex_array_buffer() override;

		/* Methods */
		virtual void add_vertex_buffer(const shared<vertex_object_buffer>& vbo) = 0;
		virtual const std::vector<shared<vertex_object_buffer>>& get_vertex_buffers() const = 0;
		virtual const shared<index_buffer>& get_index_buffer() const = 0;
		virtual void set_index_buffer(const shared<index_buffer>& ibo) = 0;

		/* Instantiate */
		static shared<vertex_array_buffer> create();
	};
}
