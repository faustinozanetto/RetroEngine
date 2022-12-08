#include "pch.h"

#include "open_gl_uniform_buffer.h"
#include "glad/glad.h"

namespace retro::renderer
{
	open_gl_uniform_buffer::open_gl_uniform_buffer(uint32_t size, uint32_t binding)
	{
		glCreateBuffers(1, &m_object_handle);
		glNamedBufferData(m_object_handle, size, nullptr, GL_DYNAMIC_DRAW);
		glBindBufferBase(GL_UNIFORM_BUFFER, binding, m_object_handle);
	}

	open_gl_uniform_buffer::~open_gl_uniform_buffer()
	{
		glDeleteBuffers(1, &m_object_handle);
	}

	void open_gl_uniform_buffer::bind()
	{
		glBindBuffer(GL_UNIFORM_BUFFER, m_object_handle);
	}

	void open_gl_uniform_buffer::un_bind()
	{
		glBindBuffer(GL_UNIFORM_BUFFER, 0);
	}

	void open_gl_uniform_buffer::set_data(const void* data, uint32_t size, uint32_t offset)
	{
		glNamedBufferSubData(m_object_handle, offset, size, data);
	}
}