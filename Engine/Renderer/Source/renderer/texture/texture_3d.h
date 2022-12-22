#pragma once

#include "texture_base.h"

namespace retro::renderer {
	class texture_3d : public texture_base
	{
	public:
		virtual ~texture_3d() = default;

		static shared<texture_3d> create(const texture_specification& texture_specification);
	};
}
