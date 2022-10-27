#include "pch.h"

#include <stb_image.h>
#include "open_gl_texture_cubemap.h"

#include "open_gl_texture.cpp"

namespace retro::renderer
{
	open_gl_texture_cubemap::open_gl_texture_cubemap(const texture_specification& texture_specification) : asset(
		asset_type::texture)
	{
		logger::line();
		m_texture_specification = texture_specification;
		logger::info("open_gl_texture_cubemap::open_gl_texture_cubemap | Loading cubemap: ");
		logger::info("  - Path: " + m_texture_specification.path);
		logger::info("  - Filtering: " + texture::get_texture_filtering_to_string(m_texture_specification.filtering));
		logger::info("  - Wrapping: " + texture::get_texture_wrapping_to_string(m_texture_specification.wrapping));

		// Variables for stb.
		int width, height, channels;
		// Load file using STB.
		stbi_set_flip_vertically_on_load(1);
		float* data = stbi_loadf(m_texture_specification.path.c_str(), &width, &height, &channels, 0);

		RETRO_CORE_ASSERT(data, "Failed to load data from image");

		// Updating size.
		m_Width = width;
		m_Height = height;
		m_Channels = channels;

		logger::info(
			"  - Width: " + std::to_string(m_Width) + " Height: " + std::to_string(
				m_Height));
		logger::info("  - Channels: " + std::to_string(m_Channels));

		m_InternalFormat = GL_RGB16F;
		m_DataFormat = GL_RGB;

		logger::info("Internal format: " + convert_texture_enum_to_string(m_InternalFormat));
		logger::info("Data format: " + convert_texture_enum_to_string(m_DataFormat));

		glCreateTextures(GL_TEXTURE_2D, 1, &m_object_handle);
		glBindTexture(GL_TEXTURE_2D, m_object_handle);

		glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB16F, width, height, 0, GL_RGB, GL_FLOAT, data);

		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
		stbi_image_free(data);
	}

	open_gl_texture_cubemap::~open_gl_texture_cubemap()
	{
		glDeleteTextures(1, &m_object_handle);
	}

	void open_gl_texture_cubemap::bind()
	{
		bind(0);
	}

	void open_gl_texture_cubemap::bind(int slot)
	{
		glBindTextureUnit(slot, m_object_handle);
	}

	void open_gl_texture_cubemap::un_bind()
	{
		glBindTexture(m_object_handle, 0);
	}
}
