#pragma once

#include "core/base.h"
#include "renderer/renderer/graphics_object.h"
#include <glm/glm.hpp>

#include "core/assets/asset.h"

namespace retro::renderer
{
	enum class texture_filtering_type
	{
		mag = 0,
		min = 1,
	};

	enum class texture_filtering
	{
		none = 0,
		linear = 1,
		nearest = 2,
	};

	enum class texture_wrapping_coords
	{
		s = 0,
		r = 1,
		t = 2,
	};

	enum class texture_wrapping
	{
		none = 0,
		repeat = 1,
		mirror_repeat = 2,
		clamp_edge = 3,
		clamp_border = 4,
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

		texture_specification(std::string path, texture_filtering filtering,
			texture_wrapping wrapping) : path(std::move(path)), filtering(filtering), wrapping(wrapping), size({ 0,0 })
		{
		}

		texture_specification(glm::uvec2 size, texture_filtering filtering,
			texture_wrapping wrapping, uint32_t format, uint32_t dataFormat) : path(""), size(size),
			filtering(filtering),
			wrapping(wrapping), format(format), dataFormat(dataFormat)
		{
		}
	};

	/**
	 * \brief Structure used when loading textures with multi-threading. Contains the information that returns
	 * stbi when loading the texture from path for later usage to create the actual texture graphics object.
	 */
	struct texture_multi_threaded
	{
		const unsigned char* data;
		int width, height, channels;

		texture_multi_threaded() = default;

		texture_multi_threaded(int width, int height, int channels, const unsigned char* data) : width(width), height(height), channels(channels), data(data) {}
	};

	class texture : public graphics_object, public asset
	{
	public:
		/* Destructor */
		~texture() override = default;

		/* Methods */

		/* Setters*/
		virtual void set_filtering(texture_filtering_type filtering_type, texture_filtering filtering) = 0;
		virtual void set_wrapping(texture_wrapping_coords wrapping_coords, texture_wrapping wrapping) = 0;

		/* Getters */
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

		static texture_multi_threaded load_texture_raw_contents(const std::string& path);

		/* Instantiate */
		static shared<texture> create(const texture_specification& texture_specification);
		static shared<texture> create(uint32_t width, uint32_t height, const unsigned char* data);
		static shared<texture> create(uint32_t width, uint32_t height, uint32_t channels, const unsigned char* data);
	};
}
