#pragma once

#include <glm/glm.hpp>

#include "core/base.h"
#include "renderer/buffers/ibo/index_buffer.h"
#include "renderer/buffers/vao/vertex_array_buffer.h"
#include "renderer/buffers/vbo/vertex_object_buffer.h"
#include "renderer/renderer/graphics_object.h"

namespace retro::renderer
{
	struct renderable_texture
	{
		unsigned int id;
		std::string type;
		std::string path;
	};

	struct renderable_vertex
	{
		glm::vec3 position;
		glm::vec2 texcoords;
		glm::vec3 normal;
		glm::vec3 tangent;
		glm::vec3 bitangent;

		renderable_vertex() : position(), texcoords(), normal(), tangent(), bitangent()
		{
		}

		renderable_vertex(const glm::vec3& position, const glm::vec2& texCoord) : position(position), texcoords(texCoord)
		{
		}

		renderable_vertex(const glm::vec3& position, const glm::vec2& texcoords, const glm::vec3& normal,
			const glm::vec3& tangent, const glm::vec3& bitangent) : position(position), texcoords(texcoords),
			normal(normal), tangent(tangent), bitangent(bitangent)
		{
		}
	};

	class renderable final : public graphics_object
	{
	public:
		/* Constructor & Destructor */
		renderable(const std::vector<renderable_vertex>& vertices, const std::vector<uint32_t>& indices,
			const std::vector<renderable_texture>& textures);
		~renderable() override = default;
		renderable(const renderable&) = delete;
		renderable& operator=(const renderable&) = delete;

		/* Methods */
		void bind() override;
		void un_bind() override;

		void construct_renderable();

		const shared<vertex_array_buffer>& get_vertex_array_buffer() const;
		const shared<vertex_object_buffer>& get_vertex_object_buffer() const;
		const shared<index_buffer>& get_index_buffer() const;
		const std::vector<renderable_vertex>& get_vertices() const;
		const std::vector<uint32_t>& get_indices() const;
		std::vector<renderable_texture> get_textures() const { return m_renderable_textures; }

		/* Instantiate */
		static shared<renderable> create(const std::vector<renderable_vertex>& vertices,
			const std::vector<uint32_t>& indices,
			const std::vector<renderable_texture>& textures);

	private:
		shared<vertex_array_buffer> m_vao;
		shared<index_buffer> m_ibo;
		shared<vertex_object_buffer> m_vbo;
		std::vector<renderable_texture> m_renderable_textures;
		std::vector<renderable_vertex> m_vertices;
		std::vector<uint32_t> m_indices;
	};
}
