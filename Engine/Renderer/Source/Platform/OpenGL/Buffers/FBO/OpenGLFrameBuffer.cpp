#include "pch.h"

#include "OpenGLFrameBuffer.h"
#include "glad/glad.h"
#include "Renderer/Textures/Texture.h"

namespace Retro::Renderer
{
    OpenGLFrameBuffer::OpenGLFrameBuffer(const FFrameBufferSpecification& frameBufferSpecification)
    {
        m_FrameBufferSpecification = frameBufferSpecification;
        // Loop through the specification and create the framebuffer
        for (auto& attachment : frameBufferSpecification.colorAttachments)
        {
            m_ColorTextureAttachmentsSpecifications.emplace_back(attachment);
        }
        Reconstruct();
    }

    OpenGLFrameBuffer::~OpenGLFrameBuffer()
    {
        glDeleteFramebuffers(1, &m_ObjectHandle);
        glDeleteTextures(m_ColorTextureAttachments.size(), m_ColorTextureAttachments.data());
        glDeleteTextures(1, &m_DepthTextureAttachment);
    }

    void OpenGLFrameBuffer::Bind()
    {
        // Bind the framebuffer.
        glViewport(0, 0, m_FrameBufferSpecification.width, m_FrameBufferSpecification.height);
        glBindFramebuffer(GL_FRAMEBUFFER, m_ObjectHandle);
    }

    void OpenGLFrameBuffer::UnBind()
    {
        glBindFramebuffer(GL_FRAMEBUFFER, 0);
    }

    void OpenGLFrameBuffer::AddColorTextureAttachment(
        const FFrameBufferColorTextureSpecification& frameBufferColorTextureAttachment)
    {
        m_ColorTextureAttachmentsSpecifications.emplace_back(frameBufferColorTextureAttachment);
        Reconstruct();
    }

    uint32_t OpenGLFrameBuffer::GetColorAttachmentID(uint32_t slot)
    {
        return m_ColorTextureAttachments[slot];
    }

    uint32_t OpenGLFrameBuffer::GetDepthAttachmentID()
    {
        return m_DepthTextureAttachment;
    }

    uint32_t OpenGLFrameBuffer::GetHeight() const
    {
        return m_FrameBufferSpecification.height;
    }

    uint32_t OpenGLFrameBuffer::GetWidth() const
    {
        return m_FrameBufferSpecification.width;
    }

    void OpenGLFrameBuffer::Resize(uint32_t newWidth, uint32_t newHeight)
    {
    }

    void OpenGLFrameBuffer::Reconstruct()
    {
        Logger::Info("OpenGLFrameBuffer::Reconstruct | Reconstructing frame buffer.");
        if (m_ObjectHandle)
        {
            glDeleteFramebuffers(1, &m_ObjectHandle);
            glDeleteTextures(m_ColorTextureAttachments.size(), m_ColorTextureAttachments.data());
            glDeleteTextures(1, &m_DepthTextureAttachment);
            m_ColorTextureAttachments.clear();
            m_DepthTextureAttachment = 0;
        }

        glCreateFramebuffers(1, &m_ObjectHandle);
        glBindFramebuffer(GL_FRAMEBUFFER, m_ObjectHandle);

        // Generate texture attachments
        if (!m_ColorTextureAttachmentsSpecifications.empty())
        {
            // Resize the array of opengl texture ids to fit the attachments.
            m_ColorTextureAttachments.resize(m_ColorTextureAttachmentsSpecifications.size());
            // Create the textures.
            glCreateTextures(GL_TEXTURE_2D, m_ColorTextureAttachments.size(), m_ColorTextureAttachments.data());

            // Generate the texture attachments.
            for (int i = 0; i < m_ColorTextureAttachments.size(); i++)
            {
                glBindTexture(GL_TEXTURE_2D, m_ColorTextureAttachments[i]);

                glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA8, m_FrameBufferSpecification.width,
                             m_FrameBufferSpecification.height, 0, GL_RGBA,
                             GL_UNSIGNED_BYTE, nullptr);
                glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
                glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

                glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_R, GL_CLAMP_TO_EDGE);
                glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
                glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);

                glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0 + i, GL_TEXTURE_2D,
                                       m_ColorTextureAttachments[i],
                                       0);
            }
        }

        // Generate depth attachment
        {
            // Create the textures.
            glCreateTextures(GL_TEXTURE_2D, 1, &m_DepthTextureAttachment);
            glBindTexture(GL_TEXTURE_2D, m_DepthTextureAttachment);

            glTexStorage2D(GL_TEXTURE_2D, 1, GL_DEPTH_COMPONENT32F, m_FrameBufferSpecification.width,
                           m_FrameBufferSpecification.height);
            glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
            glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

            glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_R, GL_CLAMP_TO_EDGE);
            glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
            glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);

            glFramebufferTexture2D(GL_FRAMEBUFFER, GL_DEPTH_ATTACHMENT, GL_TEXTURE_2D, m_DepthTextureAttachment, 0);
        }

        // Draw buffers.
        if (!m_ColorTextureAttachments.empty())
        {
            const GLenum buffers[4] = {
                GL_COLOR_ATTACHMENT0, GL_COLOR_ATTACHMENT1, GL_COLOR_ATTACHMENT2, GL_COLOR_ATTACHMENT3
            };
            glDrawBuffers(m_ColorTextureAttachments.size(), buffers);
        }
        else
        {
            // Only depth-pass
            glDrawBuffer(GL_NONE);
            glReadBuffer(GL_NONE);
        }

        const auto fboStatus = glCheckFramebufferStatus(GL_FRAMEBUFFER);
        if (fboStatus != GL_FRAMEBUFFER_COMPLETE)
        {
            Logger::Error("OpenGLFrameBuffer::Reconstruct | Framebuffer is not complete: " + std::to_string(fboStatus));
        }

        glBindFramebuffer(GL_FRAMEBUFFER, 0);
    }
}
