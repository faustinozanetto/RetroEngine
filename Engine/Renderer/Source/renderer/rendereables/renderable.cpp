#include "pch.h"

#include "renderable.h"

namespace retro::renderer
{
	renderable::renderable(const std::vector<renderable_vertex>& vertices, const std::vector<uint32_t>& indices,
	                       const std::vector<renderable_texture>& textures)
	{
		m_vertices = vertices;
		m_indices = indices;
		m_renderable_textures = textures;
		construct_renderable();
	}

	void renderable::bind()
	{
		m_vao->bind();
	}

	void renderable::un_bind()
	{
		m_vao->un_bind();
	}

	void renderable::construct_renderable()
	{
		// Create VBO, VAO and IBO
		m_vao = vertex_array_buffer::create();
		m_vbo = vertex_object_buffer::create(reinterpret_cast<float*>(&m_vertices[0]),
		                                   sizeof(renderable_vertex) * m_vertices.size());
		m_ibo = index_buffer::create((m_indices).data(), m_indices.size());
		// Setup VBO Layout
		m_vao->bind();
		m_vbo->set_layout({
			{FloatVec3, "aPos"},
			{FloatVec2, "aTexCoord"},
			{FloatVec3, "aNormal"},
			{FloatVec3, "aTangent"},
			{FloatVec3, "aBiTangent"},
		});
		m_vao->add_vertex_buffer(m_vbo);
		m_vao->set_index_buffer(m_ibo);
	}

	const shared<vertex_array_buffer>& renderable::get_vertex_array_buffer() const
	{
		return m_vao;
	}

	const shared<vertex_object_buffer>& renderable::get_vertex_object_buffer() const
	{
		return m_vbo;
	}

	const shared<index_buffer>& renderable::get_index_buffer() const
	{
		return m_ibo;
	}

	const std::vector<renderable_vertex>& renderable::get_vertices() const
	{
		return m_vertices;
	}

	const std::vector<uint32_t>& renderable::get_indices() const
	{
		return m_indices;
	}

	shared<renderable> renderable::create(const std::vector<renderable_vertex>& vertices,
	                                   const std::vector<uint32_t>& indices,
	                                   const std::vector<renderable_texture>& textures)
	{
		return create_shared<renderable>(vertices, indices, textures);
	}
}
