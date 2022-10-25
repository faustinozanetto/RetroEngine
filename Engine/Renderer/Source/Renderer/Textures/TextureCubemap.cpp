#include "pch.h"

#include "TextureCubemap.h"

#include "Platform/OpenGL/Texture/OpenGLTextureCubemap.h"
#include "Renderer/Renderer/Renderer.h"

namespace Retro::Renderer
{
	Ref<TextureCubemap> TextureCubemap::Create(const FTextureSpecification& textureSpecification)
	{
		switch (Renderer::GetRenderingAPIType())
		{
		case RenderingAPIType::None:
		{
			Logger::Error("TextureCubemap::Create | Unknown rendering api!.");
			return nullptr;
		}
		case RenderingAPIType::OpenGL:
		{
			return CreateRef<OpenGLTextureCubemap>(textureSpecification);
		}
		}
		return {};
	}
}
