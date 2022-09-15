#include "pch.h"

#include "OpenGLTexture.h"

namespace Retro::Renderer
{
    OpenGLTexture::OpenGLTexture(const FTextureSpecification& textureSpecification)
    {
        Logger::Line();
        m_TextureSpecification = textureSpecification;
        Logger::Info("OpenGLTexture::OpenGLTexture | Loading texture: ");
        Logger::Info("  - Path: " + m_TextureSpecification.path);
        Logger::Info("  - Filtering: " + GetTextureFilteringToString(m_TextureSpecification.filtering));
        Logger::Info("  - Wrapping: " + GetTextureWrappingToString(m_TextureSpecification.wrapping));

        // Variables for stb.
        int width, height, channels;
        stbi_uc* data = nullptr;

        // Load file using STB.
        stbi_set_flip_vertically_on_load(1);
        {
            data = stbi_load(m_TextureSpecification.path.c_str(), &width, &height, &channels, 0);
        }

        // Updating size.
        m_Width = width;
        m_Height = height;
        m_Channels = channels;

        Logger::Info(
            "  - Width: " + std::to_string(m_Width) + " Height: " + std::to_string(
                m_Height));
        Logger::Info("  - Channels: " + std::to_string(m_Channels));

        // Setup image formats.
        SetupTextureFormats();

        // Construct the opengl image.
        SetupImageBuffer(data);

        // Free memory.
        stbi_image_free(data);

        Logger::Line();
    }

    OpenGLTexture::~OpenGLTexture()
    {
        glDeleteTextures(1, &m_ObjectHandle);
    }

    void OpenGLTexture::Bind()
    {
        Bind(0);
    }

    void OpenGLTexture::Bind(uint32_t slot)
    {
        glBindTextureUnit(slot, m_ObjectHandle);
    }

    void OpenGLTexture::UnBind()
    {
        glBindTexture(m_ObjectHandle, 0);
    }

    const FTextureSpecification& OpenGLTexture::GetTextureSpecification() const
    {
        return m_TextureSpecification;
    }

    const uint32_t OpenGLTexture::GetMipMapLevels()
    {
        return m_MipMapLevels;
    }

    const uint32_t OpenGLTexture::GetChannels()
    {
        return m_Channels;
    }

    const uint32_t OpenGLTexture::GetImageWidth()
    {
        return m_Width;
    }

    const uint32_t OpenGLTexture::GetImageHeight()
    {
        return m_Height;
    }

    GLenum OpenGLTexture::ConvertTextureFiltering(TextureFiltering textureFiltering)
    {
        uint32_t filter = 0;
        switch (textureFiltering)
        {
        case TextureFiltering::Nearest:
            filter = GL_NEAREST;
            break;
        case TextureFiltering::Linear:
            filter = GL_LINEAR;
            break;
        case TextureFiltering::None:
            filter = 0;
            break;
        }
        return filter;
    }

    GLenum OpenGLTexture::ConvertTextureWrapping(TextureWrapping textureWrapping)
    {
        uint32_t wrap = 0;
        switch (textureWrapping)
        {
        case TextureWrapping::Repeat:
            wrap = GL_REPEAT;
            break;
        case TextureWrapping::MirrorRepeat:
            wrap = GL_MIRRORED_REPEAT;
            break;
        case TextureWrapping::ClampEdge:
            wrap = GL_CLAMP_TO_EDGE;
            break;
        case TextureWrapping::ClampBorder:
            wrap = GL_CLAMP_TO_BORDER;
            break;
        default: break;
        }
        return wrap;
    }

    void OpenGLTexture::SetupTextureFormats()
    {
        if (m_Channels == 4)
        {
            m_InternalFormat = GL_RGBA8;
            m_DataFormat = GL_RGBA;
        }
        else if (m_Channels == 3)
        {
            m_InternalFormat = GL_RGB8;
            m_DataFormat = GL_RGB;
        }
        else if (m_Channels == 2)
        {
            m_InternalFormat = GL_RG8;
            m_DataFormat = GL_RG;
        }
        else if (m_Channels == 1)
        {
            m_InternalFormat = GL_R8;
            m_DataFormat = GL_RED;
        }
        else
        {
            m_InternalFormat = GL_RGB8;
            m_DataFormat = GL_RGB;
        }
    }

    void OpenGLTexture::SetupImageBuffer(const stbi_uc* data)
    {
        // Setup mipmaps.
        m_MipMapLevels = static_cast<GLsizei>(floor(log2((std::min)(m_Width, m_Height))));

        // Generating the texture.
        glCreateTextures(GL_TEXTURE_2D, 1, &m_ObjectHandle);
        glBindTexture(GL_TEXTURE_2D, m_ObjectHandle);
        glPixelStorei(GL_UNPACK_ALIGNMENT, 1);
        glTextureStorage2D(m_ObjectHandle, m_MipMapLevels, m_InternalFormat, m_Width,
                           m_Height);

        // Filtering
        if (m_TextureSpecification.filtering != TextureFiltering::None)
        {
            const GLenum filtering = ConvertTextureFiltering(m_TextureSpecification.filtering);
            glTextureParameteri(m_ObjectHandle, GL_TEXTURE_MIN_FILTER, filtering);
            glTextureParameteri(m_ObjectHandle, GL_TEXTURE_MAG_FILTER, filtering);
        }

        // Wrapping
        if (m_TextureSpecification.wrapping != TextureWrapping::None)
        {
            const GLenum wrapping = ConvertTextureWrapping(m_TextureSpecification.wrapping);
            glTextureParameteri(m_ObjectHandle, GL_TEXTURE_WRAP_S, wrapping);
            glTextureParameteri(m_ObjectHandle, GL_TEXTURE_WRAP_T, wrapping);
        }

        // Allocating memory.
        glTextureSubImage2D(m_ObjectHandle, 0, 0, 0, m_Width, m_Height,
                            m_DataFormat, GL_UNSIGNED_BYTE, data);
        glGenerateTextureMipmap(m_ObjectHandle);
        glPixelStorei(GL_UNPACK_ALIGNMENT, 4);
    }
}
