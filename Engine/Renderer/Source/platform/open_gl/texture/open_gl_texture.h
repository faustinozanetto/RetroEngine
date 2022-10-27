#pragma once

#include "stb_image.h"
#include "core/assets/asset.h"

#include "glad/glad.h"

#include "renderer/texture/texture.h"

namespace Retro::Renderer
{
	class open_gl_texture : public texture, public asset
	{
	public:
		/* Constructor & Destructor */
		open_gl_texture(const texture_specification& texture_specification);
		open_gl_texture(uint32_t width, uint32_t height, const unsigned char* data);
		~open_gl_texture() override;

		/* Methods */
		void bind() override;
		void bind(int slot) override;
		void un_bind() override;

		const texture_specification& get_texture_specification() const override;
		int get_mip_maps_levels() override;
		int get_channels() override;
		int get_width() override;
		int get_height() override;

		/* Converts the enum value to the matching OpenGL enum. */
		static GLint convert_texture_filtering(texture_filtering texture_filtering);
		/* Converts the enum value to the matching OpenGL enum. */
		static GLint convert_texture_wrapping(texture_wrapping texture_wrapping);

	private:
		bool setup_image_formats();
		void setup_image_buffers(const stbi_uc* data);

	protected:
		texture_specification m_texture_specification;
		int m_mip_map_levels{};
		int m_channels;
		int m_width;
		int m_height;
		GLenum m_internal_format{};
		GLenum m_data_format{};
	};
}
