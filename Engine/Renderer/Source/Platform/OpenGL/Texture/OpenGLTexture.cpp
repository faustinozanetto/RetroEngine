#include "pch.h"

#include "OpenGLTexture.h"

#include "Core/Assets/Asset.h"

namespace Retro::Renderer
{
	static std::string ConvertTextureEnumToString(GLenum textureType)
	{
		switch (textureType)
		{
		case GL_SRGB8_ALPHA8:
			return "GL_SRGB8_ALPHA8";
		case GL_RGB8:
			return "GL_RGB8";
		case GL_RGBA8:
			return "GL_RGBA8";
		case GL_RGB16F:
			return "GL_RGB16F";
		case GL_RGBA16F:
			return "GL_RGBA16F";
		case GL_R8:
			return "GL_R8";
		case GL_R16F:
			return "GL_R16F";
		case GL_R32F:
			return "GL_R32F";
		case GL_RG8:
			return "GL_RG8";
		case GL_RG16F:
			return "GL_RG16F";
		case GL_RG32F:
			return "GL_RG32F";
		case GL_RED:
			return "GL_RED";
		case GL_RGB:
			return "GL_RGB";
		case GL_FLOAT:
			return "GL_FLOAT";
		case GL_RGBA:
			return "GL_RGBA";
		default:
			return "Unknown";
		}
	}

	OpenGLTexture::OpenGLTexture(const FTextureSpecification& textureSpecification) : Asset(AssetType::Texture)
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

		RETRO_CORE_ASSERT(data, "Failed to load data from image");

		// Updating size.
		m_Width = width;
		m_Height = height;
		m_Channels = channels;

		Logger::Info(
			"  - Width: " + std::to_string(m_Width) + " Height: " + std::to_string(
				m_Height));
		Logger::Info("  - Channels: " + std::to_string(m_Channels));

		// Setup texture format.
		if (!SetupTextureFormats())
		{
			Logger::Error("Could not setup texture format.");
		}

		Logger::Info("Internal format: " + ConvertTextureEnumToString(m_InternalFormat));
		Logger::Info("Data format: " + ConvertTextureEnumToString(m_DataFormat));

		// Construct the opengl image.
		SetupImageBuffer(data);

		// Free memory.
		stbi_image_free(data);

		Logger::Line();
	}

	OpenGLTexture::OpenGLTexture(uint32_t width, uint32_t height, const unsigned char* data) : Asset(AssetType::Texture)
	{
		int lWidth, lHeight, channels;
		stbi_set_flip_vertically_on_load(1);
		stbi_uc* mdata;
		if (height == 0)
		{
			mdata = stbi_load_from_memory(data, width, &lWidth, &lHeight, &channels, 0);
		}
		else
		{
			mdata = stbi_load_from_memory(data, width * height, &lWidth, &lHeight, &channels, 0);
		}

		m_Width = width;
		m_Height = height;
		m_Channels = channels;

		Logger::Info(
			"  - Width: " + std::to_string(m_Width) + " Height: " + std::to_string(
				m_Height));
		Logger::Info("  - Channels: " + std::to_string(m_Channels));

		// Setup texture format.
		if (!SetupTextureFormats())
		{
			Logger::Error("Could not setup texture format.");
		}

		Logger::Info("Internal format: " + ConvertTextureEnumToString(m_InternalFormat));
		Logger::Info("Data format: " + ConvertTextureEnumToString(m_DataFormat));

		// Generating the texture.
		glCreateTextures(GL_TEXTURE_2D, 1, &m_ObjectHandle);
		glBindTexture(GL_TEXTURE_2D, m_ObjectHandle);
		glPixelStorei(GL_UNPACK_ALIGNMENT, 1);
		glTextureStorage2D(m_ObjectHandle, m_MipMapLevels, m_InternalFormat, m_Width, m_Height);

		// Setup mipmaps.
		m_MipMapLevels = static_cast<GLsizei>(floor(log2((std::min)(m_Width, m_Height))));

		// Filtering
		if (m_TextureSpecification.filtering != TextureFiltering::None)
		{
			const GLint filtering = ConvertTextureFiltering(m_TextureSpecification.filtering);
			glTextureParameteri(m_ObjectHandle, GL_TEXTURE_MIN_FILTER, filtering);
			glTextureParameteri(m_ObjectHandle, GL_TEXTURE_MAG_FILTER, filtering);
		}

		// Wrapping
		if (m_TextureSpecification.wrapping != TextureWrapping::None)
		{
			const GLint wrapping = ConvertTextureWrapping(m_TextureSpecification.wrapping);
			glTextureParameteri(m_ObjectHandle, GL_TEXTURE_WRAP_S, wrapping);
			glTextureParameteri(m_ObjectHandle, GL_TEXTURE_WRAP_T, wrapping);
		}

		// Allocating memory.
		glTextureSubImage2D(m_ObjectHandle, 0, 0, 0, m_Width, m_Height, m_DataFormat, GL_UNSIGNED_BYTE, mdata);
		glGenerateTextureMipmap(m_ObjectHandle);
		glPixelStorei(GL_UNPACK_ALIGNMENT, 4);

		// Free memory.
		stbi_image_free(mdata);

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

	void OpenGLTexture::Bind(int slot)
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

	int OpenGLTexture::GetMipMapLevels()
	{
		return m_MipMapLevels;
	}

	int OpenGLTexture::GetChannels()
	{
		return m_Channels;
	}

	int OpenGLTexture::GetImageWidth()
	{
		return m_Width;
	}

	int OpenGLTexture::GetImageHeight()
	{
		return m_Height;
	}

	GLint OpenGLTexture::ConvertTextureFiltering(TextureFiltering textureFiltering)
	{
		GLint filter = 0;
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

	GLint OpenGLTexture::ConvertTextureWrapping(TextureWrapping textureWrapping)
	{
		GLint wrap = 0;
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
		case TextureWrapping::None: wrap = 0;
		}
		return wrap;
	}

	bool OpenGLTexture::SetupTextureFormats()
	{
		if (m_Channels == 4)
		{
			m_InternalFormat = GL_RGBA8;
			m_DataFormat = GL_RGBA;
			return true;
		}
		if (m_Channels == 3)
		{
			m_InternalFormat = GL_RGB8;
			m_DataFormat = GL_RGB;
			return true;
		}
		if (m_Channels == 2)
		{
			m_InternalFormat = GL_RG8;
			m_DataFormat = GL_RG;
			return true;
		}
		if (m_Channels == 1)
		{
			m_InternalFormat = GL_R8;
			m_DataFormat = GL_RED;
			return true;
		}
		return false;
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
			const GLint filtering = ConvertTextureFiltering(m_TextureSpecification.filtering);
			glTextureParameteri(m_ObjectHandle, GL_TEXTURE_MIN_FILTER, filtering);
			glTextureParameteri(m_ObjectHandle, GL_TEXTURE_MAG_FILTER, filtering);
		}

		// Wrapping
		if (m_TextureSpecification.wrapping != TextureWrapping::None)
		{
			const GLint wrapping = ConvertTextureWrapping(m_TextureSpecification.wrapping);
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
