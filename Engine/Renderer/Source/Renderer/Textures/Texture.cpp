#include "pch.h"

#include "Texture.h"

#include "Platform/OpenGL/Texture/OpenGLTexture.h"
#include "Renderer/Renderer/RendererContext.h"
#include "Renderer/Renderer/Renderer.h"

namespace Retro::Renderer
{
    Ref<Texture> Texture::Create(const FTextureSpecification& textureSpecification)
    {
        switch (Renderer::GetRenderingAPIType())
        {
        case RenderingAPIType::None:
            {
                Logger::Error("Texture::Create | Unknown rendering api!.");
                return nullptr;
            }
        case RenderingAPIType::OpenGL:
            {
                return CreateRef<OpenGLTexture>(textureSpecification);
            }
        }
        return {};
    }
}
