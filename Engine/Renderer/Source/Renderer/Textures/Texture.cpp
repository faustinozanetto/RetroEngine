#include "pch.h"

#include "Texture.h"

#include "Platform/OpenGL/Texture/OpenGLTexture.h"
#include "Renderer/Renderer/RendererContext.h"
#include "Renderer/Renderer/Renderer.h"

namespace Retro::Renderer
{
	std::string Texture::GetTextureFilteringToString(TextureFiltering textureFiltering)
	{
		switch (textureFiltering)
		{
		case TextureFiltering::None: return "None";
		case TextureFiltering::Linear: return "Linear";
		case TextureFiltering::Nearest: return "Nearest";
		}
		return "Unknown";
	}

	std::string Texture::GetTextureWrappingToString(TextureWrapping textureWrapping)
	{
		switch (textureWrapping)
		{
		case TextureWrapping::None: return "None";
		case TextureWrapping::Repeat: return "Repeat";
		case TextureWrapping::ClampBorder: return "ClampBorder";
		case TextureWrapping::ClampEdge: return "ClampEdge";
		case TextureWrapping::MirrorRepeat: return "MirrorRepeat";
		}
		return "Unknown";
	}

	Shared<Texture> Texture::Create(const FTextureSpecification& textureSpecification)
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
				return CreateShared<OpenGLTexture>(textureSpecification);
			}
		}
		return {};
	}

	Shared<Texture> Texture::Create(uint32_t width, uint32_t height, const unsigned char* data)
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
				return CreateShared<OpenGLTexture>(width, height, data);
			}
		}
		return {};
	}
}
