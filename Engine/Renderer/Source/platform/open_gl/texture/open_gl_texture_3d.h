#pragma once

#include "stb_image.h"

#include "glad/glad.h"

#include "renderer/texture/texture_3d.h"

namespace retro::renderer
{
	class open_gl_texture_3d final : public texture_3d, public asset
	{
	public:
		/* Constructor & Destructor */
		open_gl_texture_3d(const texture_specification& texture_specification);
		~open_gl_texture_3d() override;

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

	protected:
		texture_specification m_texture_specification{};
		int m_mip_map_levels{};
		int m_channels;
	};
}