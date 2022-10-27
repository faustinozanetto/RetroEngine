#include "pch.h"

#include "open_gl_index_buffer.h"

#include "glad/glad.h"

namespace Retro::Renderer
{
	open_gl_index_buffer::open_gl_index_buffer(uint32_t* data, uint32_t size)
	{
		// Assign size
		m_size = size;
		// Create buffer
		glGenBuffers(1, &m_object_handle);
		glBindBuffer(GL_ARRAY_BUFFER, m_object_handle);
		glBufferData(GL_ARRAY_BUFFER, size * sizeof(uint32_t), data, GL_STATIC_DRAW);
	}

	open_gl_index_buffer::~open_gl_index_buffer()
	{
		// Delete buffer
		glDeleteBuffers(1, &m_object_handle);
	}

	void open_gl_index_buffer::bind()
	{
		// Bind the buffer.
		glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, m_object_handle);
	}

	void open_gl_index_buffer::un_bind()
	{
		// Unbind the buffer.
		glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);
	}

	uint32_t open_gl_index_buffer::get_size() const
	{
		return m_size;
	}
}
