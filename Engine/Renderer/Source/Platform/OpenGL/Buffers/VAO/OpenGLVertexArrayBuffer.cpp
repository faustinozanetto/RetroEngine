#include "pch.h"

#include "OpenGLVertexArrayBuffer.h"

#include "glad/glad.h"
#include "Platform/OpenGL/Buffers/VBO/OpenGLVertexObjectBuffer.h"

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

    void OpenGLVertexArrayBuffer::AddVertexObjectBuffer(const Ref<VertexObjectBuffer>& vbo)
    {
        // Bind this VAO and then bind the VBO.
        Bind();
        vbo->Bind();

        const VBOLayout& vboLayout = vbo->GetVBOLayout();
        int index = 0;
        for (const VBOElement& vboElement : vboLayout.GetVBOElements())
        {
            Logger::Info(
                "VertexArray Element Bind: Name: " + vboElement.name + " | Index: " + std::to_string(
                    index)
                +
                " | Count : " + std::to_string(vboElement.CalculateElementCount()) + " | Type: " +
                VBOElement::GetVBOElementTypeName(vboElement.type));
            // Setup according to type.
            switch (vboElement.type)
            {
            case Float:
            case FloatVec2:
            case FloatVec3:
            case FloatVec4:
                {
                    glEnableVertexAttribArray(index);
                    glVertexAttribPointer(index,
                                          vboElement.CalculateElementCount(),
                                          OpenGLVertexObjectBuffer::GetOpenGLVBOElementType(vboElement.type),
                                          GL_FALSE,
                                          vboLayout.GetStride(),
                                          reinterpret_cast<const void*>(vboElement.offset));
                    // Increment the index
                    index++;
                    break;
                }
            }
        }

        m_VBOs.push_back(vbo);
        vbo->UnBind();
        UnBind();
    }

    const std::vector<Ref<VertexObjectBuffer>>& OpenGLVertexArrayBuffer::GetVertexObjectBuffers() const
    {
        return m_VBOs;
    }

    const Ref<IndexBuffer>& OpenGLVertexArrayBuffer::GetIndexBuffer() const
    {
        return m_IBO;
    }

    void OpenGLVertexArrayBuffer::SetIndexBuffer(const Ref<IndexBuffer>& ibo)
    {
        Bind();
        ibo->Bind();
        m_IBO = ibo;
    }
}
