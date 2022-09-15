#pragma once

#include "Core/Base.h"
#include "Renderer/Renderer/GraphicsObject.h"

namespace Retro::Renderer
{
    enum class TextureFiltering
    {
        None = 0,
        Linear = 1,
        Nearest = 2,
    };

    enum class TextureWrapping
    {
        None = 0,
        Repeat = 1,
        MirrorRepeat = 2,
        ClampEdge = 3,
        ClampBorder = 4,
    };

    enum class TextureFormat
    {
        None = 0,
        RGB = 1,
        RGBA = 2,
        RED = 3,
    };

    struct FTextureSpecification
    {
        std::string path;
        TextureFiltering filtering;
        TextureWrapping wrapping;

        FTextureSpecification() : filtering(TextureFiltering::Linear), wrapping(TextureWrapping::ClampEdge) {}

        FTextureSpecification(const std::string& path, TextureFiltering filtering, TextureWrapping wrapping) : path(path), filtering(filtering), wrapping(wrapping)
        {
        }
    };

    class Texture : public GraphicsObject
    {
    public:
        /* Destructor */
        virtual ~Texture() = default;

        /* Methods */
        virtual const FTextureSpecification& GetTextureSpecification() const = 0;
        virtual const uint32_t GetMipMapLevels() = 0;
        virtual const uint32_t GetChannels() = 0;
        virtual const uint32_t GetImageWidth() = 0;
        virtual const uint32_t GetImageHeight() = 0;

        void Bind() override = 0;
        virtual void Bind(uint32_t slot) = 0;
        void UnBind() override = 0;

        /* Instantiate */
        static Ref<Texture> Create(const FTextureSpecification& textureSpecification);
    };
}
