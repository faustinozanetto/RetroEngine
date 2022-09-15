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
        Logger::Info("  - Filtering: ");
        Logger::Info("  - Wrapping: ");

        // Variables for stb.
        int width, height, channels;
        stbi_uc* data = nullptr;

        // Load file using STB.
        stbi_set_flip_vertically_on_load(1);
        {

            data = stbi_load(m_TextureSpecification.path.c_str(), &width, &height, &channels, 0);
        }

        // Updating size.
        m_TextureSpecification.width = width;
        m_TextureSpecification.height = height;
        m_Channels = channels;

        Logger::Info(
            "  - Width: " + std::to_string(m_TextureSpecification.width) + " Height: " + std::to_string(
                m_TextureSpecification.height));
        Logger::Info("  - Channels: " + std::to_string(m_Channels));

        // Setup image formats.
        SetupTextureFormats();

        // Construct the opengl image.
        SetupImageBuffer(data);

        data = nullptr;
        
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

    void OpenGLTexture::SetupTextureFormats()
    {
        if (m_Channels == 4)
        {
            m_InternalFormat = GL_RGBA8;
            m_DataFormat = GL_RGBA;
        }
        if (m_Channels == 3)
        {
            m_InternalFormat = GL_RGB8;
            m_DataFormat = GL_RGB;
        }
        if (m_Channels == 2)
        {
            m_InternalFormat = GL_RG8;
            m_DataFormat = GL_RG;
        }
        if (m_Channels == 1)
        {
            m_InternalFormat = GL_R8;
            m_DataFormat = GL_RED;
        }
        m_InternalFormat = GL_RGB8;
        m_DataFormat = GL_RGB;
    }

    void OpenGLTexture::SetupImageBuffer(stbi_uc* data)
    {
        // Generating the texture.
        glCreateTextures(GL_TEXTURE_2D, 1, &m_ObjectHandle);
        glBindTexture(GL_TEXTURE_2D, m_ObjectHandle);
        glPixelStorei(GL_UNPACK_ALIGNMENT, 1);
        glTextureStorage2D(m_ObjectHandle, m_MipMapLevels, m_InternalFormat, m_TextureSpecification.width,
                           m_TextureSpecification.height);

        // Allocating memory.
        glTextureSubImage2D(m_ObjectHandle, 0, 0, 0, m_TextureSpecification.width, m_TextureSpecification.height,
                            m_DataFormat, GL_UNSIGNED_BYTE, data);
        glGenerateTextureMipmap(m_ObjectHandle);
        glPixelStorei(GL_UNPACK_ALIGNMENT, 4);

        // Free memory.
        stbi_image_free(data);
    }
}
