#pragma once

#include "renderer/buffers/ibo/index_buffer.h"

namespace Retro::Renderer
{
	class open_gl_index_buffer : public index_buffer
	{
	public:
		/* Constructor & Destructor */
		open_gl_index_buffer(uint32_t* data, uint32_t size);
		~open_gl_index_buffer() override;

		/* Methods */
		void bind() override;
		void un_bind() override;

		uint32_t get_size() const override;

	private:
		uint32_t m_size;
	};
}
