#pragma once

#include "texture_base.h"

namespace retro::renderer {
	class texture_2d : public texture_base
	{
	public:
		virtual ~texture_2d() = default;

		static shared<texture_2d> create(const texture_specification& texture_specification);
		static shared<texture_2d> create(uint32_t width, uint32_t height, const unsigned char* data);
		static shared<texture_2d> create(uint32_t width, uint32_t height, const void* pixels);
		static shared<texture_2d> create(uint32_t width, uint32_t height, uint32_t channels, const unsigned char* data);
	};
}
