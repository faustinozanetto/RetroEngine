﻿#pragma once

#include "renderer/buffers/ubo/uniform_buffer.h"

namespace retro::renderer
{
	class open_gl_uniform_buffer : public uniform_buffer
	{
	public:
		/* Constructor & Destructor */
		open_gl_uniform_buffer(uint32_t size, uint32_t binding);
		~open_gl_uniform_buffer() override;

		/* Methods */
		void bind() override;
		void un_bind() override;
		void set_data(const void* data, uint32_t size, uint32_t offset) override;
	};
}
