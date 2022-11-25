#pragma once

#include "core/base.h"
#include "renderer/renderer/graphics_object.h"
#include <glm/glm.hpp>

#include "core/assets/asset.h"

namespace retro::renderer
{
	enum class texture_filtering
	{
		none = 0,
		linear = 1,
		nearest = 2,
	};

	enum class texture_wrapping
	{
		none = 0,
		repeat = 1,
		mirror_repeat = 2,
		clamp_edge = 3,
		clamp_border = 4,
	};

	enum class TextureFormat
	{
		none = 0,
		rgb = 1,
		rgba = 2,
		red = 3,
	};

	struct texture_specification
	{
		std::string path;
		glm::uvec2 size;
		texture_filtering filtering = texture_filtering::linear;
		texture_wrapping wrapping = texture_wrapping::clamp_edge;
		uint32_t format;
		uint32_t dataFormat;

		texture_specification() = default;

		texture_specification(const std::string& path, texture_filtering filtering,
			texture_wrapping wrapping) : path(path), filtering(filtering), wrapping(wrapping)
		{
		}

		texture_specification(glm::uvec2 size, texture_filtering filtering,
			texture_wrapping wrapping, uint32_t format, uint32_t dataFormat) : size(size), filtering(filtering),
			wrapping(wrapping), format(format), dataFormat(dataFormat)
		{
		}
	};

	class texture : public graphics_object, public asset
	{
	public:
		/* Destructor */
		~texture() override = default;

		/* Methods */
		virtual const texture_specification& get_texture_specification() const = 0;
		virtual int get_mip_maps_levels() = 0;
		virtual int get_channels() = 0;
		virtual int get_width() = 0;
		virtual int get_height() = 0;

		void bind() override = 0;
		virtual void bind(int slot) = 0;
		void un_bind() override = 0;

		static std::string get_texture_filtering_to_string(texture_filtering texture_filtering);
		static std::string get_texture_wrapping_to_string(texture_wrapping texture_wrapping);

		/* Instantiate */
		static shared<texture> create(const texture_specification& texture_specification);
		static shared<texture> create(uint32_t width, uint32_t height, const unsigned char* data);
	};
}
