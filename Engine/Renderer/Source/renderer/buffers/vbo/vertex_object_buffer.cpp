#include "pch.h"

#include "vertex_object_buffer.h"

#include "platform/open_gl/buffers/vbo/open_gl_vertex_object_buffer.h"
#include "renderer/renderer/renderer_context.h"
#include "renderer/renderer/renderer.h"

namespace retro::renderer
{
	uint32_t layout_element::calculate_element_count() const
	{
		switch (type)
		{
		case Float: return 1;
		case FloatVec2: return 2;
		case FloatVec3: return 3;
		case FloatVec4: return 4;
		}
		return 0;
	}

	std::string layout_element::get_element_type_name(layout_element_type type)
	{
		switch (type)
		{
		case Float: return "Float";
		case FloatVec2: return "FloatVec2";
		case FloatVec3: return "FloatVec3";
		case FloatVec4: return "FloatVec4";
		}
		return "Unknown!";
	}

	layout_element::layout_element(layout_element_type type, std::string elementName)
	{
		this->type = type;
		name = elementName;
		size = CalculateElementSize(type);
		offset = 0;
	}

	vertex_buffer_layout::vertex_buffer_layout(std::initializer_list<layout_element> elements)
	{
		m_elements = elements;
		calculate_offset_and_stride();
	}

	void vertex_buffer_layout::calculate_offset_and_stride()
	{
		int offset = 0;
		m_stride = 0;
		for (auto& vboElement : m_elements)
		{
			vboElement.offset = offset;
			offset += vboElement.size;
			m_stride += vboElement.size;
		}
	}

	const std::vector<layout_element>& vertex_buffer_layout::get_elements() const
	{
		return m_elements;
	}

	uint32_t vertex_buffer_layout::get_stride() const
	{
		return m_stride;
	}

	vertex_object_buffer::~vertex_object_buffer()
	{
	}

	shared<vertex_object_buffer> vertex_object_buffer::create(float* data, uint32_t size)
	{
		switch (renderer::get_renderer_api_type())
		{
		case renderer_api_type::none:
		{
			logger::error("vertex_object_buffer::create | Unknown renderer api!.");
			return {};
		}
		case renderer_api_type::open_gl:
		{
			return create_shared<open_gl_vertex_object_buffer>(data, size);
		}
		}
		return {};
	}
}