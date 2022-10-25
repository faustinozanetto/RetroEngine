#include "pch.h"

#include "Platform/OpenGL/Buffers/IBO/OpenGLIndexBuffer.h"

#include "glad/glad.h"

namespace Retro::Renderer
{
	OpenGLIndexBuffer::OpenGLIndexBuffer(uint32_t* data, uint32_t size)
	{
		// Assign size
		m_Size = size;
		// Create buffer
		glGenBuffers(1, &m_ObjectHandle);
		glBindBuffer(GL_ARRAY_BUFFER, m_ObjectHandle);
		glBufferData(GL_ARRAY_BUFFER, size * sizeof(uint32_t), data, GL_STATIC_DRAW);
	}

	OpenGLIndexBuffer::~OpenGLIndexBuffer()
	{
		// Delete buffer
		glDeleteBuffers(1, &m_ObjectHandle);
	}

	void OpenGLIndexBuffer::Bind()
	{
		// Bind the buffer.
		glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, m_ObjectHandle);
	}

	void OpenGLIndexBuffer::UnBind()
	{
		// Unbind the buffer.
		glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);
	}

	uint32_t OpenGLIndexBuffer::GetSize() const
	{
		return m_Size;
	}
}
