#include "pch.h"
#include "OpenGLUniformBuffer.h"
#include "glad/glad.h"

namespace Retro::Renderer
{
    OpenGLUniformBuffer::OpenGLUniformBuffer(uint32_t size, uint32_t binding)
    {
        glCreateBuffers(1, &m_ObjectHandle);
        glNamedBufferData(m_ObjectHandle, size, nullptr, GL_DYNAMIC_DRAW);
        glBindBufferBase(GL_UNIFORM_BUFFER, binding, m_ObjectHandle);
    }

    OpenGLUniformBuffer::~OpenGLUniformBuffer()
    {
        glDeleteBuffers(1, &m_ObjectHandle);
    }

    void OpenGLUniformBuffer::Bind()
    {
    }

    void OpenGLUniformBuffer::UnBind()
    {
    }

    void OpenGLUniformBuffer::SetData(const void* data, uint32_t size, uint32_t offset)
    {
        glNamedBufferSubData(m_ObjectHandle, offset, size, data);
    }
}
