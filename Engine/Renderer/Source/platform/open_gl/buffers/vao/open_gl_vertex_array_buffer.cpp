#include "pch.h"

#include "open_gl_vertex_array_buffer.h"

#include "glad/glad.h"

namespace Retro::Renderer
{
	open_gl_vertex_array_buffer::open_gl_vertex_array_buffer()
	{
		// Create vertex array.
		glGenVertexArrays(1, &m_object_handle);
		// Bind the vertex array
		glBindVertexArray(m_object_handle);
	}

	open_gl_vertex_array_buffer::~open_gl_vertex_array_buffer()
	{
		glDeleteVertexArrays(1, &m_object_handle);
	}

	void open_gl_vertex_array_buffer::bind()
	{
		glBindVertexArray(m_object_handle);
	}

	void open_gl_vertex_array_buffer::un_bind()
	{
		glBindVertexArray(0);
	}

	void open_gl_vertex_array_buffer::add_vertex_buffer(const shared<vertex_object_buffer>& vbo)
	{
		// Bind this VAO and then bind the VBO.
		bind();
		vbo->bind();

		const vertex_buffer_layout& vboLayout = vbo->get_layout();
		int index = 0;
		for (const layout_element& vboElement : vboLayout.get_elements())
		{
			logger::info(
				"VertexArray Element Bind: Name: " + vboElement.name + " | Index: " + std::to_string(
					index)
				+
				" | Count : " + std::to_string(vboElement.calculate_element_count()) + " | Type: " +
				layout_element::get_element_type_name(vboElement.type));
			// Setup according to type.
			switch (vboElement.type)
			{
			case Float:
			case FloatVec2:
			case FloatVec3:
			case FloatVec4:
				{
					glEnableVertexAttribArray(index);
					glVertexAttribPointer(index,
					                      vboElement.calculate_element_count(),
						open_gl_vertex_object_buffer::get_open_gl_layout_element_type(vboElement.type),
					                      GL_FALSE,
					                      vboLayout.get_stride(),
					                      reinterpret_cast<const void*>(vboElement.offset));
					// Increment the index
					index++;
					break;
				}
			case Int:
			case IntVec2:
			case IntVec3:
			case IntVec4:
				{
					glEnableVertexAttribArray(index);
					glVertexAttribIPointer(index,
					                       vboElement.calculate_element_count(),
						open_gl_vertex_object_buffer::get_open_gl_layout_element_type(vboElement.type),
					                       vboLayout.get_stride(),
					                       reinterpret_cast<const void*>(vboElement.offset));
					// Increment the index
					index++;
					break;
				}
			case Mat3:
			case Mat4:
				{
					// Get count and loop
					const uint32_t size = vboElement.calculate_element_count();
					for (uint8_t i = 0; i < size; i++)
					{
						// Enable the attribute
						glEnableVertexAttribArray(index);
						glVertexAttribPointer(index, size,
							open_gl_vertex_object_buffer::get_open_gl_layout_element_type(vboElement.type),
						                      GL_FALSE, vboLayout.get_stride(),
						                      reinterpret_cast<const void*>(sizeof(float) * size * i));
						glVertexAttribDivisor(index, 1);
						// Increment the index
						index++;
					}
					break;
				}
			}
		}

		m_vbos.push_back(vbo);
		vbo->un_bind();
		un_bind();
	}

	const std::vector<shared<vertex_object_buffer>>& open_gl_vertex_array_buffer::get_vertex_buffers() const
	{
		return m_vbos;
	}

	const shared<index_buffer>& open_gl_vertex_array_buffer::get_index_buffer() const
	{
		return m_ibo;
	}

	void open_gl_vertex_array_buffer::set_index_buffer(const shared<index_buffer>& ibo)
	{
		bind();
		ibo->bind();
		m_ibo = ibo;
	}
}
