#pragma once

#include "stb_image.h"

#include "glad/glad.h"

#include "Renderer/Textures/Texture.h"

namespace Retro::Renderer
{
    class OpenGLTexture : public Texture
    {
    public:
        /* Constructor & Destructor */
        OpenGLTexture(const FTextureSpecification& textureSpecification);
        ~OpenGLTexture() override;

        /* Methods */
        void Bind() override;
        void Bind(uint32_t slot) override;
        void UnBind() override;

        const FTextureSpecification& GetTextureSpecification() const override;
        const uint32_t GetMipMapLevels() override;
        const uint32_t GetChannels() override;
        const uint32_t GetImageWidth() override;
        const uint32_t GetImageHeight() override;

    private:
        /* Converts the enum value to the matching OpenGL enum. */
        GLenum ConvertTextureFiltering(TextureFiltering textureFiltering);
        /* Converts the enum value to the matching OpenGL enum. */
        GLenum ConvertTextureWrapping(TextureWrapping textureWrapping);
        
        void SetupTextureFormats();
        void SetupImageBuffer(const stbi_uc* data);
        
    private:
        FTextureSpecification m_TextureSpecification;
        uint32_t m_MipMapLevels;
        uint32_t m_Channels;
        uint32_t m_Width;
        uint32_t m_Height;
        GLenum m_InternalFormat;
        GLenum m_DataFormat;
    };
}
