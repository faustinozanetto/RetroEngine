#include "pch.h"

#include "OpenGLVertexArrayBuffer.h"

#include "glad/glad.h"

namespace Retro::Renderer
{
	OpenGLVertexArrayBuffer::OpenGLVertexArrayBuffer()
	{
		// Create vertex array.
		glGenVertexArrays(1, &m_ObjectHandle);
		// Bind the vertex array
		glBindVertexArray(m_ObjectHandle);
	}

	OpenGLVertexArrayBuffer::~OpenGLVertexArrayBuffer()
	{
		glDeleteVertexArrays(1, &m_ObjectHandle);
	}

	void OpenGLVertexArrayBuffer::Bind()
	{
		glBindVertexArray(m_ObjectHandle);
	}

	void OpenGLVertexArrayBuffer::UnBind()
	{
		glBindVertexArray(0);
	}
}