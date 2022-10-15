#include "pch.h"
#include "OpenGLUniformBuffer.h"
#include "glad/glad.h"

namespace Retro::Renderer
{
    OpenGLUniformBuffer::OpenGLUniformBuffer(uint32_t size, uint32_t binding)
    {
        //glCreateBuffers(1, &m_ObjectHandle);
       // glNamedBufferData(m_ObjectHandle, size, nullptr, GL_DYNAMIC_DRAW);
        //glBindBufferBase(GL_UNIFORM_BUFFER, binding, m_ObjectHandle);
        glCreateBuffers(1, &m_ObjectHandle);
        glBindBuffer(GL_UNIFORM_BUFFER, m_ObjectHandle);
    }

    OpenGLUniformBuffer::~OpenGLUniformBuffer()
    {
        glDeleteBuffers(1, &m_ObjectHandle);
    }

    void OpenGLUniformBuffer::Bind()
    {
        glBindBuffer(GL_UNIFORM_BUFFER, m_ObjectHandle);
    }

    void OpenGLUniformBuffer::UnBind()
    {
        glBindBuffer(GL_UNIFORM_BUFFER, 0);
    }

    void OpenGLUniformBuffer::SetData(const void* data, uint32_t size, uint32_t offset)
    {
        //glNamedBufferSubData(m_ObjectHandle, offset, size, data);
        glBufferSubData(GL_UNIFORM_BUFFER, offset, size, data);
    }

    void OpenGLUniformBuffer::SetIBOLayout(const VBOLayout& layout, uint32_t block, uint32_t count) {
        m_Layout = layout;
        uint32_t size = 0;
        for (const auto& element : layout.GetVBOElements())
            size += CalculateElementSize(element.type);

        size *= count;

        glBindBuffer(GL_UNIFORM_BUFFER, m_ObjectHandle);
        glBufferData(GL_UNIFORM_BUFFER, size, nullptr, GL_STATIC_DRAW);
        glBindBuffer(GL_UNIFORM_BUFFER, 0);
        glBindBufferRange(GL_UNIFORM_BUFFER, block, m_ObjectHandle, 0, size);
    }
}
