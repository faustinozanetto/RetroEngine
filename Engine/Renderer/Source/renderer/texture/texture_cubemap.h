#pragma once

#include "core/base.h"
#include "renderer/texture/texture_base.h"
#include "renderer/renderer/graphics_object.h"

namespace retro::renderer
{
	class texture_cubemap : public graphics_object
	{
	public:
		/* Destructor */
		~texture_cubemap() override = default;

		/* Methods */
		void bind() override = 0;
		virtual void bind(int slot) = 0;
		void un_bind() override = 0;

		static shared<texture_cubemap> create(const texture_specification& texture_specification);
	};
}
