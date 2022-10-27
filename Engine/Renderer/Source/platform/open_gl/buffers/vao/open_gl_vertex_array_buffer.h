#pragma once

#include "renderer/buffers/ibo/index_buffer.h"
#include "renderer/buffers/vao/vertex_array_buffer.h"

namespace retro::renderer
{
	class open_gl_vertex_array_buffer : public vertex_array_buffer
	{
	public:
		/* Constructor & Destructor */
		open_gl_vertex_array_buffer();
		~open_gl_vertex_array_buffer() override;

		/* Methods */
		void bind() override;
		void un_bind() override;

		void add_vertex_buffer(const shared<vertex_object_buffer>& vbo) override;
		const std::vector<shared<vertex_object_buffer>>& get_vertex_buffers() const override;
		const shared<index_buffer>& get_index_buffer() const override;
		void set_index_buffer(const shared<index_buffer>& ibo) override;

	private:
		std::vector<shared<vertex_object_buffer>> m_vbos;
		shared<index_buffer> m_ibo;
	};
}
