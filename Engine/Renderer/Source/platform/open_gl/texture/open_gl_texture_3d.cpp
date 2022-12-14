#include "pch.h"

#include "open_gl_texture_3d.h"
#include "open_gl_texture_2d.h"
#include "renderer/renderer/render_passes/global_illumination_pass.h"

namespace retro::renderer
{
	open_gl_texture_3d::open_gl_texture_3d(const texture_specification& texture_specification)
	{
		logger::line();
		m_texture_specification = texture_specification;
		logger::info(
			"  - Width: " + std::to_string(m_texture_specification.size.x) + " Height: " + std::to_string(
				m_texture_specification.size.y));
		logger::info("  - Channels: " + std::to_string(m_channels));

		// Generating the texture.
		glGenTextures(1, &m_object_handle);
		glBindTexture(GL_TEXTURE_3D, m_object_handle);

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
			set_wrapping(texture_wrapping_coords::t, m_texture_specification.wrapping);
		}

		const int levels = 7;
		glTexStorage3D(GL_TEXTURE_3D, levels, GL_RGBA8, m_texture_specification.size.x, m_texture_specification.size.x, m_texture_specification.size.x);

		//const int data_size = m_texture_specification.size.x * m_texture_specification.size.x * m_texture_specification.size.x;
		//GLubyte* data = new GLubyte[4 * data_size];
		//memset(data, 0, 4 * data_size);

		const std::vector<GLfloat> textureBuffer(4 * VOXEL_SIZE * VOXEL_SIZE * VOXEL_SIZE, 0.0f);;

		glTexImage3D(GL_TEXTURE_3D, 0, m_texture_specification.format, m_texture_specification.size.x, m_texture_specification.size.x,
			m_texture_specification.size.x, 0, m_texture_specification.dataFormat, GL_FLOAT, &textureBuffer[0]);

		glGenerateMipmap(GL_TEXTURE_3D);
		glBindTexture(GL_TEXTURE_3D, 0);

		logger::line();
	}

	open_gl_texture_3d::~open_gl_texture_3d()
	{
		glDeleteTextures(1, &m_object_handle);
	}

	void open_gl_texture_3d::bind()
	{
		bind(0);
	}

	void open_gl_texture_3d::bind(int slot)
	{
		glBindTextureUnit(slot, m_object_handle);
	}

	void open_gl_texture_3d::un_bind()
	{
		glBindTexture(m_object_handle, 0);
	}

	void open_gl_texture_3d::set_filtering(texture_filtering_type filtering_type, texture_filtering filtering)
	{
		glTextureParameteri(m_object_handle, open_gl_texture_2d::convert_texture_filtering_type(filtering_type),
			open_gl_texture_2d::convert_texture_filtering(filtering));
	}

	void open_gl_texture_3d::set_wrapping(texture_wrapping_coords wrapping_coords, texture_wrapping wrapping)
	{
		glTextureParameteri(m_object_handle, open_gl_texture_2d::convert_texture_wrapping_coords(wrapping_coords),
			open_gl_texture_2d::convert_texture_wrapping(wrapping));
	}

	texture_specification& open_gl_texture_3d::get_texture_specification()
	{
		return m_texture_specification;
	}

	int open_gl_texture_3d::get_mip_maps_levels()
	{
		return m_mip_map_levels;
	}

	int open_gl_texture_3d::get_channels()
	{
		return m_channels;
	}

	int open_gl_texture_3d::get_width()
	{
		return m_texture_specification.size.x;
	}

	int open_gl_texture_3d::get_height()
	{
		return m_texture_specification.size.y;
	}
}