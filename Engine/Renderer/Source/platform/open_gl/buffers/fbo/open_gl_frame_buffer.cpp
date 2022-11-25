#include "pch.h"

#include "open_gl_frame_buffer.h"

#include "core/assert.h"
#include "renderer/texture/texture.h"
#include "glad/glad.h"
#include "platform/open_gl/texture/open_gl_texture.h"

namespace retro::renderer
{
    void open_gl_frame_buffer::generate_color_texture(uint32_t texture_handle, int index,
                                                      frame_buffer_texture_specification texture_specification)
    {
        glTexImage2D(GL_TEXTURE_2D, 0, texture_specification.specification.dataFormat,
                     m_frame_buffer_specification.width,
                     m_frame_buffer_specification.height, 0, texture_specification.specification.format,
                     texture_specification.specification.dataFormat == GL_RGBA16F ? GL_FLOAT : GL_UNSIGNED_BYTE,
                     nullptr);

        // Filtering
        if (texture_specification.specification.filtering != texture_filtering::none)
        {
            const GLint filtering = open_gl_texture::convert_texture_filtering(
                texture_specification.specification.filtering);
            glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, filtering);
            glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, filtering);
        }

        // Wrapping
        if (texture_specification.specification.wrapping != texture_wrapping::none)
        {
            const GLint wrapping = open_gl_texture::convert_texture_wrapping(
                texture_specification.specification.wrapping);
            glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_R, wrapping);
            glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, wrapping);
            glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, wrapping);
        }

        glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0 + index, GL_TEXTURE_2D,
                               texture_handle,
                               0);
    }

    void open_gl_frame_buffer::generate_depth_texture(uint32_t texture_handle,
                                                      frame_buffer_texture_specification texture_specification)
    {
        glTexStorage2D(GL_TEXTURE_2D, 1, GL_DEPTH_COMPONENT32F, m_frame_buffer_specification.width,
                       m_frame_buffer_specification.height);

        // Filtering
        if (texture_specification.specification.filtering != texture_filtering::none)
        {
            const GLint filtering = open_gl_texture::convert_texture_filtering(
                texture_specification.specification.filtering);
            glTextureParameteri(m_object_handle, GL_TEXTURE_MIN_FILTER, filtering);
            glTextureParameteri(m_object_handle, GL_TEXTURE_MAG_FILTER, filtering);
        }

        // Wrapping
        if (texture_specification.specification.wrapping != texture_wrapping::none)
        {
            const GLint wrapping = open_gl_texture::convert_texture_wrapping(
                texture_specification.specification.wrapping);
            glTextureParameteri(m_object_handle, GL_TEXTURE_WRAP_R, wrapping);
            glTextureParameteri(m_object_handle, GL_TEXTURE_WRAP_S, wrapping);
            glTextureParameteri(m_object_handle, GL_TEXTURE_WRAP_T, wrapping);
        }

        glFramebufferTexture2D(GL_FRAMEBUFFER, GL_DEPTH_ATTACHMENT, GL_TEXTURE_2D, texture_handle, 0);
    }

    open_gl_frame_buffer::open_gl_frame_buffer(const frame_buffer_specification& frame_buffer_specification)
    {
        m_frame_buffer_specification = frame_buffer_specification;
        // Loop through the specification and create the framebuffer
        for (auto& attachment : m_frame_buffer_specification.attachments)
        {
            if (attachment.specification.format == GL_DEPTH_COMPONENT32F)
            {
                m_depth_texture_specification = attachment;
            }
            else
            {
                m_frame_buffer_texture_specifications.emplace_back(attachment);
            }
        }
        reconstruct();
    }

    open_gl_frame_buffer::~open_gl_frame_buffer()
    {
        glDeleteFramebuffers(1, &m_object_handle);
        glDeleteTextures(m_attachments.size(), m_attachments.data());
        glDeleteTextures(1, &m_depth_attachment);
    }

    void open_gl_frame_buffer::bind()
    {
        // Bind the framebuffer.
        glViewport(0, 0, m_frame_buffer_specification.width, m_frame_buffer_specification.height);
        glBindFramebuffer(GL_FRAMEBUFFER, m_object_handle);
    }

    void open_gl_frame_buffer::un_bind()
    {
        glBindFramebuffer(GL_FRAMEBUFFER, 0);
    }

    void open_gl_frame_buffer::add_texture_attachment(
        const frame_buffer_texture_specification& frame_buffer_texture_specification)
    {
        m_frame_buffer_texture_specifications.emplace_back(frame_buffer_texture_specification);
        reconstruct();
    }

    uint32_t open_gl_frame_buffer::get_attachment_id(uint32_t slot)
    {
        return m_attachments[slot];
    }

    std::vector<uint32_t> open_gl_frame_buffer::get_attachments()
    {
        return m_attachments;
    }

    std::map<uint32_t, frame_buffer_texture_specification>
    open_gl_frame_buffer::get_attachments_specifications()
    {
        std::map<uint32_t, frame_buffer_texture_specification> result = {};
        int i = 0;
        for (auto& attachment : m_attachments)
        {
            result.insert({attachment, m_frame_buffer_texture_specifications.at(i)});
            i++;
        }
        return result;
    }

    uint32_t open_gl_frame_buffer::get_depth_attachment_id()
    {
        return m_depth_attachment;
    }

    uint32_t open_gl_frame_buffer::get_height() const
    {
        return m_frame_buffer_specification.height;
    }

    uint32_t open_gl_frame_buffer::get_width() const
    {
        return m_frame_buffer_specification.width;
    }

    void open_gl_frame_buffer::resize(uint32_t newWidth, uint32_t newHeight)
    {
        m_frame_buffer_specification.width = newWidth;
        m_frame_buffer_specification.height = newHeight;
        reconstruct();
    }

    void open_gl_frame_buffer::reconstruct()
    {
        logger::info("open_gl_frame_buffer::reconstruct | Reconstructing frame buffer.");
        if (m_object_handle)
        {
            glDeleteFramebuffers(1, &m_object_handle);
            glDeleteTextures(m_attachments.size(), m_attachments.data());
            glDeleteTextures(1, &m_depth_attachment);
            m_attachments.clear();
            m_depth_attachment = 0;
        }

        glCreateFramebuffers(1, &m_object_handle);
        glBindFramebuffer(GL_FRAMEBUFFER, m_object_handle);

        // Generate texture attachments
        if (!m_frame_buffer_texture_specifications.empty())
        {
            // Resize the array of opengl texture ids to fit the attachments.
            m_attachments.resize(m_frame_buffer_texture_specifications.size());
            // Create the textures.
            glCreateTextures(GL_TEXTURE_2D, m_attachments.size(),
                             m_attachments.data());

            // Generate the texture attachments.
            for (int i = 0; i < m_attachments.size(); i++)
            {
                glBindTexture(GL_TEXTURE_2D, m_attachments[i]);
                generate_color_texture(m_attachments[i], i, m_frame_buffer_texture_specifications[i]);
            }
        }

        glCreateTextures(GL_TEXTURE_2D, 1, &m_depth_attachment);
        glBindTexture(GL_TEXTURE_2D, m_depth_attachment);
        generate_depth_texture(m_depth_attachment, m_depth_texture_specification);

        // Draw buffers.
        if (m_attachments.size() > 1)
        {
            const GLenum buffers[8] = {
                GL_COLOR_ATTACHMENT0, GL_COLOR_ATTACHMENT1, GL_COLOR_ATTACHMENT2, GL_COLOR_ATTACHMENT3,
                GL_COLOR_ATTACHMENT4, GL_COLOR_ATTACHMENT5, GL_COLOR_ATTACHMENT6, GL_COLOR_ATTACHMENT7
            };
            glDrawBuffers(m_attachments.size(), buffers);
        }
        else if (m_attachments.empty())
        {
            // Only depth-pass
            glDrawBuffer(GL_NONE);
            glReadBuffer(GL_NONE);
        }

        const auto fboStatus = glCheckFramebufferStatus(GL_FRAMEBUFFER);
        RETRO_CORE_ASSERT(fboStatus == GL_FRAMEBUFFER_COMPLETE,
                          "open_gl_frame_buffer::reconstruct | Frame buffer is not complete: " + std::to_string(
                              fboStatus))

        glBindFramebuffer(GL_FRAMEBUFFER, 0);
    }
}
