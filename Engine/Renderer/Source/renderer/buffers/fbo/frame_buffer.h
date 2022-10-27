#pragma once

#include "core/base.h"
#include "renderer/renderer/graphics_object.h"

namespace retro::renderer
{
	enum class frame_buffer_color_attachment_format
	{
		rgba8 = 0,
		rgba16f = 1,
	};

	struct frame_buffer_color_texture_specification
	{
		frame_buffer_color_attachment_format format;

		frame_buffer_color_texture_specification(frame_buffer_color_attachment_format format) : format(format)
		{
		}
	};

	struct frame_buffer_specification
	{
		uint32_t width;
		uint32_t height;

		std::vector<frame_buffer_color_texture_specification> color_attachments;

		frame_buffer_specification() : width(1920), height(1080), color_attachments({})
		{
		}

		frame_buffer_specification(uint32_t width, uint32_t height,
		                           std::initializer_list<frame_buffer_color_texture_specification> color_attachments) :
			width(width),
			height(height), color_attachments(color_attachments)
		{
		}
	};

	class frame_buffer : public graphics_object
	{
	public:
		/* Destructor */
		~frame_buffer() override;

		/* Methods */
		void bind() override = 0;
		void un_bind() override = 0;

		virtual void add_color_texture_attachment(
			const frame_buffer_color_texture_specification& frame_buffer_color_texture_specification) = 0;

		virtual void resize(uint32_t newWidth, uint32_t newHeight) = 0;
		virtual uint32_t get_color_attachment_id(uint32_t slot = 0) = 0;
		virtual uint32_t get_deptch_attachment_id() = 0;
		virtual uint32_t get_width() const = 0;
		virtual uint32_t get_height() const = 0;

		/* Instantiate */
		static shared<frame_buffer> create(const frame_buffer_specification& frame_buffer_specification);
	};
}
