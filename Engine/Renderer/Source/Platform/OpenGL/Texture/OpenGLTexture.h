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

    private:
        void SetupTextureFormats();
        void SetupImageBuffer(stbi_uc* data);

    private:
        FTextureSpecification m_TextureSpecification;
        uint32_t m_MipMapLevels;
        uint32_t m_Channels;
        GLenum m_InternalFormat;
        GLenum m_DataFormat;
    };
}
