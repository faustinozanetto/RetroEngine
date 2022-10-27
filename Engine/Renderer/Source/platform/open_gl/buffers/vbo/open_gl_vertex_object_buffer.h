#pragma once

#include "renderer/buffers/vbo/vertex_object_buffer.h"

namespace Retro::Renderer
{
	class open_gl_vertex_object_buffer : public vertex_object_buffer
	{
	public:
		/* Constructor & Destructor */
		open_gl_vertex_object_buffer(const float* vertices, uint32_t size);
		~open_gl_vertex_object_buffer() override;

		/* Methods */
		void bind() override;
		void un_bind() override;

		const vertex_buffer_layout& get_layout() const override;
		void set_layout(const vertex_buffer_layout& layout) override;

		static uint32_t get_open_gl_layout_element_type(layout_element_type vboElementType);
	private:
		vertex_buffer_layout m_vertex_buffer_layout;
	};
}
