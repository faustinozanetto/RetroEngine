#include "pch.h"

#include "open_gl_frame_buffer.h"

#include "core/assert.h"
#include "renderer/texture/texture.h"
#include "glad/glad.h"

namespace retro::renderer
{
    void open_gl_frame_buffer::generate_color_texture(uint32_t texture_handle, int index, uint32_t width,
                                                      uint32_t height,
                                                      GLenum format, GLenum dataFormat)
    {
        glTexImage2D(GL_TEXTURE_2D, 0, dataFormat, width,
                     height, 0, format,
                     dataFormat == GL_RGBA16F ? GL_FLOAT : GL_UNSIGNED_BYTE, nullptr);

        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_R, GL_CLAMP_TO_EDGE);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);

        glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0 + index, GL_TEXTURE_2D,
                               texture_handle,
                               0);
    }

    void open_gl_frame_buffer::generate_depth_texture(uint32_t texture_handle, uint32_t width, uint32_t height)
    {
        glTexStorage2D(GL_TEXTURE_2D, 1, GL_DEPTH_COMPONENT32F, width,
                       height);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_R, GL_CLAMP_TO_EDGE);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);

        glFramebufferTexture2D(GL_FRAMEBUFFER, GL_DEPTH_ATTACHMENT, GL_TEXTURE_2D, texture_handle, 0);
    }

    open_gl_frame_buffer::open_gl_frame_buffer(const frame_buffer_specification& frame_buffer_specification)
    {
        m_frame_buffer_specification = frame_buffer_specification;
        // Loop through the specification and create the framebuffer
        for (auto& attachment : m_frame_buffer_specification.color_attachments)
        {
            m_frame_buffer_color_texture_specifications.emplace_back(attachment);
        }
        reconstruct();
    }

    open_gl_frame_buffer::~open_gl_frame_buffer()
    {
        glDeleteFramebuffers(1, &m_object_handle);
        glDeleteTextures(m_color_attachments.size(), m_color_attachments.data());
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

    void open_gl_frame_buffer::add_color_texture_attachment(
        const frame_buffer_color_texture_specification& frame_buffer_color_texture_specification)
    {
        m_frame_buffer_color_texture_specifications.emplace_back(frame_buffer_color_texture_specification);
        reconstruct();
    }

    uint32_t open_gl_frame_buffer::get_color_attachment_id(uint32_t slot)
    {
        return m_color_attachments[slot];
    }

    std::vector<uint32_t> open_gl_frame_buffer::get_color_attachments()
    {
        return m_color_attachments;
    }

    std::map<uint32_t, frame_buffer_color_texture_specification>
    open_gl_frame_buffer::get_color_attachments_specifications()
    {
        std::map<uint32_t, frame_buffer_color_texture_specification> result = {};
        int i = 0;
        for (auto& attachment : m_color_attachments)
        {
            result.insert({attachment, m_frame_buffer_color_texture_specifications.at(i)});
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
            glDeleteTextures(m_color_attachments.size(), m_color_attachments.data());
            glDeleteTextures(1, &m_depth_attachment);
            m_color_attachments.clear();
            m_depth_attachment = 0;
        }

        glCreateFramebuffers(1, &m_object_handle);
        glBindFramebuffer(GL_FRAMEBUFFER, m_object_handle);

        // Generate texture attachments
        if (!m_frame_buffer_color_texture_specifications.empty())
        {
            // Resize the array of opengl texture ids to fit the attachments.
            m_color_attachments.resize(m_frame_buffer_color_texture_specifications.size());
            // Create the textures.
            glCreateTextures(GL_TEXTURE_2D, m_color_attachments.size(),
                             m_color_attachments.data());

            // Generate the texture attachments.
            for (int i = 0; i < m_color_attachments.size(); i++)
            {
                glBindTexture(GL_TEXTURE_2D, m_color_attachments[i]);

                GLenum format;
                GLenum dataFormat;
                switch (m_frame_buffer_color_texture_specifications[i].format)
                {
                case frame_buffer_color_attachment_format::rgba8:
                    {
                        format = GL_RGBA;
                        dataFormat = GL_RGBA8;
                        break;
                    }
                case frame_buffer_color_attachment_format::rgba16f:
                    {
                        format = GL_RGBA;
                        dataFormat = GL_RGB16F;
                        break;
                    }
                }

                generate_color_texture(m_color_attachments[i], i, m_frame_buffer_specification.width,
                                       m_frame_buffer_specification.height, format, dataFormat);
            }
        }

        // Generate depth attachment
        {
            // Create the textures.
            glCreateTextures(GL_TEXTURE_2D, 1, &m_depth_attachment);
            glBindTexture(GL_TEXTURE_2D, m_depth_attachment);
            generate_depth_texture(m_depth_attachment, m_frame_buffer_specification.width,
                                   m_frame_buffer_specification.height);
        }

        // Draw buffers.
        if (!m_color_attachments.empty())
        {
            const GLenum buffers[8] = {
                GL_COLOR_ATTACHMENT0, GL_COLOR_ATTACHMENT1, GL_COLOR_ATTACHMENT2, GL_COLOR_ATTACHMENT3,
                GL_COLOR_ATTACHMENT4, GL_COLOR_ATTACHMENT5,GL_COLOR_ATTACHMENT6,GL_COLOR_ATTACHMENT7
            };
            glDrawBuffers(m_color_attachments.size(), buffers);
        }

        const auto fboStatus = glCheckFramebufferStatus(GL_FRAMEBUFFER);
        if (fboStatus != GL_FRAMEBUFFER_COMPLETE)
        {
            logger::error(
                "open_gl_frame_buffer::reconstruct | Frame buffer is not complete: " + std::to_string(fboStatus));
        }

        glBindFramebuffer(GL_FRAMEBUFFER, 0);
    }
}
