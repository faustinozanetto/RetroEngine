#include "pch.h"
#include "OpenGLVertexBuffer.h"

#include "glad/glad.h"

namespace Retro::Renderer
{
    OpenGLVertexBuffer::OpenGLVertexBuffer(const float* vertices, uint32_t size)
    {
        glGenBuffers(1, &m_ObjectHandle);
        glBindBuffer(GL_ARRAY_BUFFER, m_ObjectHandle);
        glBufferData(GL_ARRAY_BUFFER, size, vertices, GL_STATIC_DRAW);
    }

    OpenGLVertexBuffer::~OpenGLVertexBuffer()
    {
        glDeleteBuffers(1, &m_ObjectHandle);
    }

    void OpenGLVertexBuffer::Bind()
    {
        glBindBuffer(GL_ARRAY_BUFFER, m_ObjectHandle);
    }

    void OpenGLVertexBuffer::UnBind()
    {
        glBindBuffer(GL_ARRAY_BUFFER, 0);
    }
}
