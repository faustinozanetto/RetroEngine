#include "pch.h"

#include "open_gl_vertex_object_buffer.h"

#include "glad/glad.h"

namespace Retro::Renderer
{
	open_gl_vertex_object_buffer::open_gl_vertex_object_buffer(const float* vertices, uint32_t size)
	{
		glGenBuffers(1, &m_object_handle);
		glBindBuffer(GL_ARRAY_BUFFER, m_object_handle);
		glBufferData(GL_ARRAY_BUFFER, size, vertices, GL_STATIC_DRAW);
	}

	open_gl_vertex_object_buffer::~open_gl_vertex_object_buffer()
	{
		glDeleteBuffers(1, &m_object_handle);
	}

	void open_gl_vertex_object_buffer::bind()
	{
		glBindBuffer(GL_ARRAY_BUFFER, m_object_handle);
	}

	void open_gl_vertex_object_buffer::un_bind()
	{
		glBindBuffer(GL_ARRAY_BUFFER, 0);
	}

	const vertex_buffer_layout& open_gl_vertex_object_buffer::get_layout() const
	{
		return m_vertex_buffer_layout;
	}

	void open_gl_vertex_object_buffer::set_layout(const vertex_buffer_layout& layout)
	{
		m_vertex_buffer_layout = layout;
	}

	uint32_t open_gl_vertex_object_buffer::get_open_gl_layout_element_type(layout_element_type vboElementType)
	{
		switch (vboElementType)
		{
		case Float: return GL_FLOAT;
		case FloatVec2: return GL_FLOAT;
		case FloatVec3: return GL_FLOAT;
		case FloatVec4: return GL_FLOAT;
		}
		return NULL;
	}
}
