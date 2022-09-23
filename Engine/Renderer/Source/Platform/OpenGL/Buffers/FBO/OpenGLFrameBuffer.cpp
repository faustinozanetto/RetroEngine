#include "pch.h"

#include "OpenGLFrameBuffer.h"
#include "glad/glad.h"

namespace Retro::Renderer
{
    OpenGLFrameBuffer::OpenGLFrameBuffer(const FFrameBufferSpecification& frameBufferSpecification)
    {
        m_FrameBufferSpecification = frameBufferSpecification;
        glGenFramebuffers(1, &m_ObjectHandle);
    }

    OpenGLFrameBuffer::~OpenGLFrameBuffer()
    {
        glDeleteFramebuffers(1, &m_ObjectHandle);
    }

    void OpenGLFrameBuffer::Bind()
    {
        glBindFramebuffer(GL_FRAMEBUFFER, m_ObjectHandle);
        glViewport(0, 0, m_FrameBufferSpecification.width, m_FrameBufferSpecification.height);
    }

    void OpenGLFrameBuffer::UnBind()
    {
        glBindFramebuffer(GL_FRAMEBUFFER, 0);
    }

    uint32_t OpenGLFrameBuffer::GetHeight() const
    {
    }

    uint32_t OpenGLFrameBuffer::GetWidth() const
    {
    }

    void OpenGLFrameBuffer::Resize(uint32_t newWidth, uint32_t newHeight)
    {
    }
}
