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
        TextureFiltering filtering;
        TextureWrapping wrapping;
        TextureFormat format;
        std::string path;
        uint32_t mipMapLevels;
        uint32_t width;
        uint32_t height;
    };
    
    class Texture : public GraphicsObject
    {
    public:
        /* Destructor */
        virtual ~Texture() = default;

        /* Methods */
        virtual const FTextureSpecification& GetTextureSpecification() const = 0;
        
        void Bind() override = 0;
        void UnBind() override = 0;

        /* Instantiate */
        static Ref<Texture> Create(const FTextureSpecification& textureSpecification);
    };
}
