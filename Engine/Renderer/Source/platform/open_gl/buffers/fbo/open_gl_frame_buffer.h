#pragma once

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

		void add_texture_attachment(
			const frame_buffer_texture_specification& frame_buffer_texture_specification) override;

		uint32_t get_attachment_id(uint32_t slot) override;
		std::vector<uint32_t> get_attachments() override;
		std::map<uint32_t, frame_buffer_texture_specification> get_attachments_specifications() override;
		uint32_t get_depth_attachment_id() override;
		uint32_t get_height() const override;
		uint32_t get_width() const override;

		void resize(uint32_t newWidth, uint32_t newHeight) override;

	private:
		void reconstruct();

		void generate_color_texture(uint32_t texture_handle, int index, frame_buffer_texture_specification texture_specification);
		void generate_depth_texture(uint32_t texture_handle, frame_buffer_texture_specification texture_specification);

	private:
		frame_buffer_specification m_frame_buffer_specification;
		std::vector<frame_buffer_texture_specification> m_frame_buffer_texture_specifications;
		std::vector<uint32_t> m_attachments;
		frame_buffer_texture_specification m_depth_texture_specification;
		uint32_t m_depth_attachment;
	};
}
