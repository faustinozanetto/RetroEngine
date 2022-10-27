#include "pch.h"

#include "open_gl_uniform_buffer.h"
#include "glad/glad.h"

namespace Retro::Renderer
{
	open_gl_uniform_buffer::open_gl_uniform_buffer(uint32_t size, uint32_t binding)
	{
		//glCreateBuffers(1, &m_ObjectHandle);
		// glNamedBufferData(m_ObjectHandle, size, nullptr, GL_DYNAMIC_DRAW);
		//glBindBufferBase(GL_UNIFORM_BUFFER, binding, m_ObjectHandle);
		glCreateBuffers(1, &m_object_handle);
		glBindBuffer(GL_UNIFORM_BUFFER, m_object_handle);
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
		//glNamedBufferSubData(m_ObjectHandle, offset, size, data);
		glBufferSubData(GL_UNIFORM_BUFFER, offset, size, data);
	}

	void open_gl_uniform_buffer::set_layout(const vertex_buffer_layout& layout, uint32_t block, uint32_t count)
	{
		m_layout = layout;
		uint32_t size = 0;
		for (const auto& element : layout.get_elements())
			size += CalculateElementSize(element.type);

		size *= count;

		glBindBuffer(GL_UNIFORM_BUFFER, m_object_handle);
		glBufferData(GL_UNIFORM_BUFFER, size, nullptr, GL_STATIC_DRAW);
		glBindBuffer(GL_UNIFORM_BUFFER, 0);
		glBindBufferRange(GL_UNIFORM_BUFFER, block, m_object_handle, 0, size);
	}
}
