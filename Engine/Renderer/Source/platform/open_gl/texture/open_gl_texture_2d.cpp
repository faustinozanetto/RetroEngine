#include "pch.h"

#include "open_gl_texture_2d.h"
#include "core/assert.h"
#include "core/assets/asset.h"

namespace retro::renderer
{
	static std::string convert_texture_enum_to_string(GLenum texture_type)
	{
		switch (texture_type)
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

	open_gl_texture_2d::open_gl_texture_2d(const texture_specification& texture_specification) : asset(asset_type::texture)
	{
		logger::line();
		m_texture_specification = texture_specification;
		if (!m_texture_specification.path.empty()) setup_image_from_path();
		else setup_image_no_path();
		logger::line();
	}

	open_gl_texture_2d::open_gl_texture_2d(uint32_t width, uint32_t height, const unsigned char* data) : asset(
		asset_type::texture)
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
		m_texture_specification.size = glm::vec2(lWidth, lHeight);
		m_channels = channels;

		logger::info(
			"  - Width: " + std::to_string(m_texture_specification.size.x) + " Height: " + std::to_string(
				m_texture_specification.size.y));
		logger::info("  - Channels: " + std::to_string(m_channels));

		// Setup texture format.
		if (!setup_image_formats())
		{
			logger::error("Could not setup texture format.");
		}

		logger::info("Internal format: " + convert_texture_enum_to_string(m_texture_specification.format));
		logger::info("Data format: " + convert_texture_enum_to_string(m_texture_specification.dataFormat));

		// Setup mipmaps.
		m_mip_map_levels = static_cast<GLsizei>(floor(
			log2((std::max)(m_texture_specification.size.x, m_texture_specification.size.y))));

		// Generating the texture.
		glCreateTextures(GL_TEXTURE_2D, 1, &m_object_handle);
		glBindTexture(GL_TEXTURE_2D, m_object_handle);
		glPixelStorei(GL_UNPACK_ALIGNMENT, 1);
		glTextureStorage2D(m_object_handle, m_mip_map_levels, m_texture_specification.format,
			m_texture_specification.size.x, m_texture_specification.size.y);

		// Filtering
		if (m_texture_specification.filtering != texture_filtering::none)
		{
			const GLint filtering = convert_texture_filtering(m_texture_specification.filtering);
			glTextureParameteri(m_object_handle, GL_TEXTURE_MIN_FILTER, filtering);
			glTextureParameteri(m_object_handle, GL_TEXTURE_MAG_FILTER, filtering);
		}

		// Wrapping
		if (m_texture_specification.wrapping != texture_wrapping::none)
		{
			const GLint wrapping = convert_texture_wrapping(m_texture_specification.wrapping);
			glTextureParameteri(m_object_handle, GL_TEXTURE_WRAP_S, wrapping);
			glTextureParameteri(m_object_handle, GL_TEXTURE_WRAP_T, wrapping);
		}

		// Allocating memory.
		glTextureSubImage2D(m_object_handle, 0, 0, 0, m_texture_specification.size.x, m_texture_specification.size.y,
			m_texture_specification.dataFormat, GL_UNSIGNED_BYTE, mdata);
		glGenerateTextureMipmap(m_object_handle);
		glPixelStorei(GL_UNPACK_ALIGNMENT, 4);

		// Free memory.
		stbi_image_free(mdata);

		logger::line();
	}

	open_gl_texture_2d::open_gl_texture_2d(uint32_t width, uint32_t height, const void* pixels)
	{
		m_texture_specification.size = glm::vec2(width, height);
		m_channels = 3;

		logger::info(
			"  - Width: " + std::to_string(m_texture_specification.size.x) + " Height: " + std::to_string(
				m_texture_specification.size.y));
		logger::info("  - Channels: " + std::to_string(m_channels));

		// Setup texture format.
		if (!setup_image_formats())
		{
			logger::error("Could not setup texture format.");
		}

		logger::info("Internal format: " + convert_texture_enum_to_string(m_texture_specification.format));
		logger::info("Data format: " + convert_texture_enum_to_string(m_texture_specification.dataFormat));

		// Setup mipmaps.
		m_mip_map_levels = static_cast<GLsizei>(floor(
			log2((std::max)(m_texture_specification.size.x, m_texture_specification.size.y))));

		// Generating the texture.
		glCreateTextures(GL_TEXTURE_2D, 1, &m_object_handle);
		glBindTexture(GL_TEXTURE_2D, m_object_handle);
		glPixelStorei(GL_UNPACK_ALIGNMENT, 1);
		glTextureStorage2D(m_object_handle, m_mip_map_levels, m_texture_specification.format,
			m_texture_specification.size.x, m_texture_specification.size.y);

		// Filtering
		if (m_texture_specification.filtering != texture_filtering::none)
		{
			const GLint filtering = convert_texture_filtering(m_texture_specification.filtering);
			glTextureParameteri(m_object_handle, GL_TEXTURE_MIN_FILTER, filtering);
			glTextureParameteri(m_object_handle, GL_TEXTURE_MAG_FILTER, filtering);
		}

		// Wrapping
		if (m_texture_specification.wrapping != texture_wrapping::none)
		{
			const GLint wrapping = convert_texture_wrapping(m_texture_specification.wrapping);
			glTextureParameteri(m_object_handle, GL_TEXTURE_WRAP_S, wrapping);
			glTextureParameteri(m_object_handle, GL_TEXTURE_WRAP_T, wrapping);
		}

		// Allocating memory.
		glTextureSubImage2D(m_object_handle, 0, 0, 0, m_texture_specification.size.x, m_texture_specification.size.y,
			m_texture_specification.dataFormat, GL_UNSIGNED_BYTE, pixels);
		glGenerateTextureMipmap(m_object_handle);
		glPixelStorei(GL_UNPACK_ALIGNMENT, 4);

		logger::line();
	}

	open_gl_texture_2d::open_gl_texture_2d(uint32_t width, uint32_t height, uint32_t channels, const unsigned char* data) : asset(
		asset_type::texture)
	{
		m_texture_specification.size = glm::vec2(width, height);
		m_channels = channels;

		if (width == 0)
		{
			logger::info("width = 0");
		}

		logger::info(
			"  - Width: " + std::to_string(m_texture_specification.size.x) + " Height: " + std::to_string(
				m_texture_specification.size.y));
		logger::info("  - Channels: " + std::to_string(m_channels));

		// Setup texture format.
		if (!setup_image_formats())
		{
			logger::error("Could not setup texture format.");
		}

		logger::info("Internal format: " + convert_texture_enum_to_string(m_texture_specification.format));
		logger::info("Data format: " + convert_texture_enum_to_string(m_texture_specification.dataFormat));

		// Setup mipmaps.
		m_mip_map_levels = static_cast<GLsizei>(floor(
			log2((std::max)(m_texture_specification.size.x, m_texture_specification.size.y))));

		// Generating the texture.
		glCreateTextures(GL_TEXTURE_2D, 1, &m_object_handle);
		glBindTexture(GL_TEXTURE_2D, m_object_handle);
		glPixelStorei(GL_UNPACK_ALIGNMENT, 1);
		glTextureStorage2D(m_object_handle, m_mip_map_levels, m_texture_specification.format,
			m_texture_specification.size.x, m_texture_specification.size.y);

		// Filtering
		if (m_texture_specification.filtering != texture_filtering::none)
		{
			const GLint filtering = convert_texture_filtering(m_texture_specification.filtering);
			glTextureParameteri(m_object_handle, GL_TEXTURE_MIN_FILTER, filtering);
			glTextureParameteri(m_object_handle, GL_TEXTURE_MAG_FILTER, filtering);
		}

		// Wrapping
		if (m_texture_specification.wrapping != texture_wrapping::none)
		{
			const GLint wrapping = convert_texture_wrapping(m_texture_specification.wrapping);
			glTextureParameteri(m_object_handle, GL_TEXTURE_WRAP_S, wrapping);
			glTextureParameteri(m_object_handle, GL_TEXTURE_WRAP_T, wrapping);
		}

		// Allocating memory.
		glTextureSubImage2D(m_object_handle, 0, 0, 0, m_texture_specification.size.x, m_texture_specification.size.y,
			m_texture_specification.dataFormat, GL_UNSIGNED_BYTE, data);
		glGenerateTextureMipmap(m_object_handle);
		glPixelStorei(GL_UNPACK_ALIGNMENT, 4);

		logger::line();
	}

	open_gl_texture_2d::~open_gl_texture_2d()
	{
		glDeleteTextures(1, &m_object_handle);
	}

	void open_gl_texture_2d::bind()
	{
		bind(0);
	}

	void open_gl_texture_2d::bind(int slot)
	{
		glBindTextureUnit(slot, m_object_handle);
	}

	void open_gl_texture_2d::un_bind()
	{
		glBindTexture(m_object_handle, 0);
	}

	void open_gl_texture_2d::set_filtering(texture_filtering_type filtering_type, texture_filtering filtering)
	{
		glTextureParameteri(m_object_handle, convert_texture_filtering_type(filtering_type),
			convert_texture_filtering(filtering));
	}

	void open_gl_texture_2d::set_wrapping(texture_wrapping_coords wrapping_coords, texture_wrapping wrapping)
	{
		glTextureParameteri(m_object_handle, convert_texture_wrapping_coords(wrapping_coords),
			convert_texture_wrapping(wrapping));
	}

	texture_specification& open_gl_texture_2d::get_texture_specification()
	{
		return m_texture_specification;
	}

	int open_gl_texture_2d::get_mip_maps_levels()
	{
		return m_mip_map_levels;
	}

	int open_gl_texture_2d::get_channels()
	{
		return m_channels;
	}

	int open_gl_texture_2d::get_width()
	{
		return m_texture_specification.size.x;
	}

	int open_gl_texture_2d::get_height()
	{
		return m_texture_specification.size.y;
	}

	GLint open_gl_texture_2d::convert_texture_filtering(texture_filtering texture_filtering)
	{
		GLint filter = 0;
		switch (texture_filtering)
		{
		case texture_filtering::nearest: {
			filter = GL_NEAREST;
			break;
		}
		case texture_filtering::linear: {
			filter = GL_LINEAR;
			break;
		}
		case texture_filtering::linear_mipmap_nearest: {
			filter = GL_LINEAR_MIPMAP_NEAREST;
			break;
		}
		case texture_filtering::linear_mipmap_linear: {
			filter = GL_LINEAR_MIPMAP_LINEAR;
			break;
		}
		case texture_filtering::nearest_mipmap_nearest: {
			filter = GL_NEAREST_MIPMAP_NEAREST;
			break;
		}
		case texture_filtering::nearest_mipmap_linear: {
			filter = GL_NEAREST_MIPMAP_LINEAR;
			break;
		}
		case texture_filtering::none: {
			filter = 0;
			break;
		}
		}
		return filter;
	}

	GLint open_gl_texture_2d::convert_texture_wrapping(texture_wrapping texture_wrapping)
	{
		GLint wrap = 0;
		switch (texture_wrapping)
		{
		case texture_wrapping::repeat: {
			wrap = GL_REPEAT;
			break;
		}
		case texture_wrapping::mirror_repeat: {
			wrap = GL_MIRRORED_REPEAT;
			break;
		}
		case texture_wrapping::clamp_edge: {
			wrap = GL_CLAMP_TO_EDGE;
			break;
		}
		case texture_wrapping::clamp_border: {
			wrap = GL_CLAMP_TO_BORDER;
			break;
		}
		case texture_wrapping::none: wrap = 0;
		}
		return wrap;
	}

	GLint open_gl_texture_2d::convert_texture_wrapping_coords(texture_wrapping_coords wrapping_coords)
	{
		switch (wrapping_coords)
		{
		case texture_wrapping_coords::r:
			return GL_TEXTURE_WRAP_R;
		case texture_wrapping_coords::s:
			return GL_TEXTURE_WRAP_S;
		case texture_wrapping_coords::t:
			return GL_TEXTURE_WRAP_T;
		}
	}

	GLint open_gl_texture_2d::convert_texture_filtering_type(texture_filtering_type filtering_type)
	{
		switch (filtering_type)
		{
		case texture_filtering_type::mag:
			return GL_TEXTURE_MAG_FILTER;
		case texture_filtering_type::min:
			return GL_TEXTURE_MIN_FILTER;
		}
	}

	void open_gl_texture_2d::setup_image_from_path()
	{
		logger::info("OpenGLTexture::OpenGLTexture | Loading texture from path: ");
		logger::info("  - Path: " + m_texture_specification.path);
		logger::info("  - Filtering: " + get_texture_filtering_to_string(m_texture_specification.filtering));
		logger::info("  - Wrapping: " + get_texture_wrapping_to_string(m_texture_specification.wrapping));

		// Variables for stb.
		int width, height, channels;
		stbi_uc* data;

		// Load file using STB.
		stbi_set_flip_vertically_on_load(1);
		{
			data = stbi_load(m_texture_specification.path.c_str(), &width, &height, &channels, 0);
		}

		RETRO_CORE_ASSERT(data, "Failed to load data from image");

		// Updating size.
		m_texture_specification.size = { width, height };
		m_channels = channels;

		logger::info(
			"  - Width: " + std::to_string(m_texture_specification.size.x) + " Height: " + std::to_string(
				m_texture_specification.size.y));
		logger::info("  - Channels: " + std::to_string(m_channels));

		// Setup texture format.
		if (!setup_image_formats())
		{
			logger::error("Could not setup texture format.");
		}

		logger::info("  - Internal format: " + convert_texture_enum_to_string(m_texture_specification.format));
		logger::info("  - Data format: " + convert_texture_enum_to_string(m_texture_specification.dataFormat));

		// Construct the opengl image.
		setup_image_buffers(data);

		// Free memory.
		stbi_image_free(data);
	}

	void open_gl_texture_2d::setup_image_no_path()
	{
		logger::info("OpenGLTexture::OpenGLTexture | Loading texture: ");
		logger::info("  - Path: None");
		logger::info("  - Filtering: " + get_texture_filtering_to_string(m_texture_specification.filtering));
		logger::info("  - Wrapping: " + get_texture_wrapping_to_string(m_texture_specification.wrapping));

		m_channels = 4;

		logger::info(
			"  - Width: " + std::to_string(m_texture_specification.size.x) + " Height: " + std::to_string(
				m_texture_specification.size.y));
		logger::info("  - Channels: " + std::to_string(m_channels));

		logger::info("  - Internal format: " + convert_texture_enum_to_string(m_texture_specification.format));
		logger::info("  - Data format: " + convert_texture_enum_to_string(m_texture_specification.dataFormat));

		// Setup mipmaps.
		m_mip_map_levels = static_cast<GLsizei>(floor(
			log2((std::min)(m_texture_specification.size.x, m_texture_specification.size.y))));

		// Generating the texture.
		glGenTextures(1, &m_object_handle);
		glActiveTexture(GL_TEXTURE0);
		glBindTexture(GL_TEXTURE_2D, m_object_handle);

		// Filtering
		if (m_texture_specification.filtering != texture_filtering::none)
		{
			const GLint filtering = convert_texture_filtering(m_texture_specification.filtering);
			glTextureParameteri(m_object_handle, GL_TEXTURE_MIN_FILTER, filtering);
			glTextureParameteri(m_object_handle, GL_TEXTURE_MAG_FILTER, filtering);
		}

		// Wrapping
		if (m_texture_specification.wrapping != texture_wrapping::none)
		{
			const GLint wrapping = convert_texture_wrapping(m_texture_specification.wrapping);
			glTextureParameteri(m_object_handle, GL_TEXTURE_WRAP_S, wrapping);
			glTextureParameteri(m_object_handle, GL_TEXTURE_WRAP_T, wrapping);
		}
		if (m_texture_specification.format == GL_DEPTH_COMPONENT) {
			glTexImage2D(GL_TEXTURE_2D, 0, m_texture_specification.format, m_texture_specification.size.x, m_texture_specification.size.y, 0,
				m_texture_specification.dataFormat, GL_FLOAT, NULL);
		}
		else
		{
			glTexImage2D(GL_TEXTURE_2D, 0, m_texture_specification.format, m_texture_specification.size.x, m_texture_specification.size.y, 0,
				m_texture_specification.dataFormat, GL_UNSIGNED_BYTE, NULL);
		}

		glBindImageTexture(0, m_object_handle, 0, GL_FALSE, 0, GL_READ_WRITE, m_texture_specification.format);
	}

	bool open_gl_texture_2d::setup_image_formats()
	{
		if (m_channels == 4)
		{
			m_texture_specification.format = GL_RGBA8;
			m_texture_specification.dataFormat = GL_RGBA;
			return true;
		}
		if (m_channels == 3)
		{
			m_texture_specification.format = GL_RGB8;
			m_texture_specification.dataFormat = GL_RGB;
			return true;
		}
		if (m_channels == 2)
		{
			m_texture_specification.format = GL_RG8;
			m_texture_specification.dataFormat = GL_RG;
			return true;
		}
		if (m_channels == 1)
		{
			m_texture_specification.format = GL_R8;
			m_texture_specification.dataFormat = GL_RED;
			return true;
		}
		return false;
	}

	void open_gl_texture_2d::setup_image_buffers(const stbi_uc* data)
	{
		// Setup mipmaps.
		m_mip_map_levels = static_cast<GLsizei>(floor(
			log2((std::min)(m_texture_specification.size.x, m_texture_specification.size.y))));

		// Generating the texture.
		glCreateTextures(GL_TEXTURE_2D, 1, &m_object_handle);
		glBindTexture(GL_TEXTURE_2D, m_object_handle);
		glPixelStorei(GL_UNPACK_ALIGNMENT, 1);
		glTextureStorage2D(m_object_handle, m_mip_map_levels, m_texture_specification.format,
			m_texture_specification.size.x,
			m_texture_specification.size.y);

		// Filtering
		if (m_texture_specification.filtering != texture_filtering::none)
		{
			set_filtering(texture_filtering_type::min, m_texture_specification.filtering);
			set_filtering(texture_filtering_type::mag, m_texture_specification.filtering);
		}

		// Wrapping
		if (m_texture_specification.wrapping != texture_wrapping::none)
		{
			set_wrapping(texture_wrapping_coords::r, m_texture_specification.wrapping);
			set_wrapping(texture_wrapping_coords::s, m_texture_specification.wrapping);
		}

		// Allocating memory.
		glTextureSubImage2D(m_object_handle, 0, 0, 0, m_texture_specification.size.x, m_texture_specification.size.y,
			m_texture_specification.dataFormat, GL_UNSIGNED_BYTE, data);
		glGenerateTextureMipmap(m_object_handle);
		glPixelStorei(GL_UNPACK_ALIGNMENT, 4);
	}
}