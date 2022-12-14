#pragma once

#include "stb_image.h"

#include "glad/glad.h"

#include "renderer/texture/texture_2d.h"

namespace retro::renderer
{
	class open_gl_texture_2d final : public texture_2d, public asset
	{
	public:
		/* Constructor & Destructor */
		open_gl_texture_2d(const texture_specification& texture_specification);
		open_gl_texture_2d(uint32_t width, uint32_t height, const unsigned char* data);
		open_gl_texture_2d(uint32_t width, uint32_t height, uint32_t channels, const unsigned char* data);
		~open_gl_texture_2d() override;

		/* Methods */
		void bind() override;
		void bind(int slot) override;
		void un_bind() override;

		void set_filtering(texture_filtering_type filtering_type, texture_filtering filtering) override;
		void set_wrapping(texture_wrapping_coords wrapping_coords, texture_wrapping wrapping) override;

		texture_specification& get_texture_specification() override;
		int get_mip_maps_levels() override;
		int get_channels() override;
		int get_width() override;
		int get_height() override;

		/* Converts the enum value to the matching OpenGL enum. */
		static GLint convert_texture_filtering(texture_filtering texture_filtering);
		/* Converts the enum value to the matching OpenGL enum. */
		static GLint convert_texture_wrapping(texture_wrapping texture_wrapping);
		/* Converts the enum value to the matching OpenGL enum. */
		static GLint convert_texture_wrapping_coords(texture_wrapping_coords wrapping_coords);
		/* Converts the enum value to the matching OpenGL enum. */
		static GLint convert_texture_filtering_type(texture_filtering_type filtering_type);

	private:
		bool setup_image_formats();
		void setup_image_buffers(const stbi_uc* data);
		void setup_image_from_path();
		void setup_image_no_path();
	protected:
		texture_specification m_texture_specification{};
		int m_mip_map_levels{};
		int m_channels;
	};
}