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

    const VBOLayout& OpenGLVertexObjectBuffer::GetVBOLayout() const
    {
        return m_VBOLayout;
    }

    void OpenGLVertexObjectBuffer::SetVBOLayout(const VBOLayout& layout)
    {
        m_VBOLayout = layout;
    }

    uint32_t OpenGLVertexObjectBuffer::GetOpenGLVBOElementType(VBOElementType vboElementType)
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
