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
        void Bind(int slot) override;
        void UnBind() override;

        const FTextureSpecification& GetTextureSpecification() const override;
        int GetMipMapLevels() override;
        int GetChannels() override;
        int GetImageWidth() override;
        int GetImageHeight() override;

        /* Converts the enum value to the matching OpenGL enum. */
        static GLint ConvertTextureFiltering(TextureFiltering textureFiltering);
        /* Converts the enum value to the matching OpenGL enum. */
        static GLint ConvertTextureWrapping(TextureWrapping textureWrapping);

    private:
        void SetupTextureFormats();
        void SetupImageBuffer(const stbi_uc* data);

    private:
        FTextureSpecification m_TextureSpecification;
        int m_MipMapLevels;
        int m_Channels;
        int m_Width;
        int m_Height;
        GLenum m_InternalFormat;
        GLenum m_DataFormat;
    };
}
