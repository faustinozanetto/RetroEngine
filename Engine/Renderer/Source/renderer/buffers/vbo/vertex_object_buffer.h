#pragma once

#include "core/base.h"
#include "renderer/renderer/graphics_object.h"

namespace Retro::Renderer
{
	enum layout_element_type
	{
		Float = 0,
		FloatVec2 = 1,
		FloatVec3 = 2,
		FloatVec4 = 3,
		Int = 4,
		IntVec2 = 5,
		IntVec3 = 6,
		IntVec4 = 7,
		Mat3 = 8,
		Mat4 = 9
	};

	static uint32_t CalculateElementSize(layout_element_type type)
	{
		switch (type)
		{
		case Float: return 4;
		case FloatVec2: return 4 * 2;
		case FloatVec3: return 4 * 3;
		case FloatVec4: return 4 * 4;
		case Mat3: return 4 * 3 * 3;
		case Mat4: return 4 * 4 * 4;
		case Int: return 4;
		case IntVec2: return 4 * 2;
		case IntVec3: return 4 * 3;
		case IntVec4: return 4 * 4;
		}
		return 0;
	}

	struct layout_element
	{
		layout_element_type type;
		std::string name;
		uint32_t size;
		uint32_t offset;

		/* Constructors */
		layout_element() = default;

		layout_element(layout_element_type type, std::string elementName);

		uint32_t calculate_element_count() const;
		static std::string get_element_type_name(layout_element_type type);
	};

	class vertex_buffer_layout
	{
	public:
		/* Constructors & Destructors */
		vertex_buffer_layout() = default;
		vertex_buffer_layout(std::initializer_list<layout_element> elements);
		~vertex_buffer_layout() = default;

		/* Methods */
		void calculate_offset_and_stride();
		const std::vector<layout_element>& get_elements() const;
		uint32_t get_stride() const;

	private:
		std::vector<layout_element> m_elements;
		uint32_t m_stride;
	};

	class vertex_object_buffer : public graphics_object
	{
	public:
		/* Destructor */
		~vertex_object_buffer() override;

		/* Methods */
		virtual const vertex_buffer_layout& get_layout() const = 0;
		virtual void set_layout(const  vertex_buffer_layout& vertex_buffer_layout) = 0;

		/* Instantiate */
		static shared<vertex_object_buffer> create(float* data, uint32_t size);
	};
}
