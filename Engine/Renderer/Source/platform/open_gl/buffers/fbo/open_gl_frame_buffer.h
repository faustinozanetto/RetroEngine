﻿#pragma once

#include "glad/glad.h"
#include "renderer/buffers/fbo/frame_buffer.h"

namespace retro::renderer
{
	class open_gl_frame_buffer : public frame_buffer
	{
	public:
		/* Constructor & Destructor */
		open_gl_frame_buffer(const frame_buffer_specification& frame_buffer_specification);
		~open_gl_frame_buffer() override;

		/* Methods */
		void bind() override;
		void un_bind() override;

		void add_color_texture_attachment(
			const frame_buffer_color_texture_specification& frame_buffer_color_texture_specification) override;

		uint32_t get_color_attachment_id(uint32_t slot) override;
		uint32_t get_deptch_attachment_id() override;
		uint32_t get_height() const override;
		uint32_t get_width() const override;

		void resize(uint32_t newWidth, uint32_t newHeight) override;

	private:
		void reconstruct();

		void generate_color_texture(uint32_t texture_handle, int index, uint32_t width, uint32_t height, GLenum format,
		                          GLenum dataFormat);

		void generate_depth_texture(uint32_t texture_handle, uint32_t width, uint32_t height);

	private:
		frame_buffer_specification m_frame_buffer_specification;
		std::vector<frame_buffer_color_texture_specification> m_frame_buffer_color_texture_specifications;
		std::vector<uint32_t> m_color_attachments;
		uint32_t m_depth_attachment;
	};
}