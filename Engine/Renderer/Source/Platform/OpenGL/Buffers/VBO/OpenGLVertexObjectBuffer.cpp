#include "pch.h"
#include "OpenGLVertexObjectBuffer.h"

#include "glad/glad.h"

namespace Retro::Renderer
{
    OpenGLVertexObjectBuffer::OpenGLVertexObjectBuffer(const float* vertices, uint32_t size)
    {
        glGenBuffers(1, &m_ObjectHandle);
        glBindBuffer(GL_ARRAY_BUFFER, m_ObjectHandle);
        glBufferData(GL_ARRAY_BUFFER, size, vertices, GL_STATIC_DRAW);
    }

    OpenGLVertexObjectBuffer::~OpenGLVertexObjectBuffer()
    {
        glDeleteBuffers(1, &m_ObjectHandle);
    }

    void OpenGLVertexObjectBuffer::Bind()
    {
        glBindBuffer(GL_ARRAY_BUFFER, m_ObjectHandle);
    }

    void OpenGLVertexObjectBuffer::UnBind()
    {
        glBindBuffer(GL_ARRAY_BUFFER, 0);
    }
}
