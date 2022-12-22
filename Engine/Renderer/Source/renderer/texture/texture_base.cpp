#include "pch.h"

#include "texture_base.h"

#include "stb_image.h"

namespace retro::renderer
{
	std::string texture_base::get_texture_filtering_to_string(texture_filtering texture_filtering)
	{
		switch (texture_filtering)
		{
		case texture_filtering::none: return "None";
		case texture_filtering::linear: return "Linear";
		case texture_filtering::nearest: return "Nearest";
		case texture_filtering::linear_mipmap_nearest: return "Linear Mipmap Nearest";
		case texture_filtering::linear_mipmap_linear: return "Linear Mipmap Linear";
		case texture_filtering::nearest_mipmap_nearest: return "Nearest Mipmap Nearest";
		case texture_filtering::nearest_mipmap_linear: return "Nearest Mipmap Linear";
		default:;
		}
		return "Unknown";
	}

	std::string texture_base::get_texture_wrapping_to_string(texture_wrapping texture_wrapping)
	{
		switch (texture_wrapping)
		{
		case texture_wrapping::none: return "None";
		case texture_wrapping::repeat: return "Repeat";
		case texture_wrapping::clamp_border: return "ClampBorder";
		case texture_wrapping::clamp_edge: return "ClampEdge";
		case texture_wrapping::mirror_repeat: return "MirrorRepeat";
		}
		return "Unknown";
	}

	texture_multi_threaded texture_base::load_texture_raw_contents(const std::string& path)
	{
		// Variables for stb.
		int width, height, channels;
		stbi_uc* data;
		// Load file using STB.
		stbi_set_flip_vertically_on_load(1);
		{
			data = stbi_load(path.c_str(), &width, &height, &channels, 0);
		}
		return { path, width, height, channels, data };
	}
}