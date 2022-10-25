#pragma once

#include <glm/glm.hpp>

#include "Core/Base.h"
#include "Renderer/Buffers/IBO/IndexBuffer.h"
#include "Renderer/Buffers/VAO/VertexArrayBuffer.h"
#include "Renderer/Buffers/VBO/VertexObjectBuffer.h"
#include "Renderer/Renderer/GraphicsObject.h"

namespace Retro::Renderer
{
	struct RendereableTexture
	{
		unsigned int id;
		std::string type;
		std::string path;
	};

	struct RenderableVertex
	{
		glm::vec3 position;
		glm::vec2 texCoord;
		glm::vec3 normal;
		glm::vec3 tangent;
		glm::vec3 bitangent;

		RenderableVertex(): position(), texCoord(), normal(), tangent(), bitangent()
		{
		}

		RenderableVertex(const glm::vec3& position, const glm::vec2& texCoord) : position(position), texCoord(texCoord)
		{
		}

		RenderableVertex(const glm::vec3& position, const glm::vec2& texCoord, const glm::vec3& normal,
		                 const glm::vec3& tangent, const glm::vec3& bitangent) : position(position), texCoord(texCoord),
			normal(normal), tangent(tangent), bitangent(bitangent)
		{
		}
	};

	class Renderable : public GraphicsObject
	{
	public:
		/* Constructor & Destructor */
		Renderable(const std::vector<RenderableVertex>& vertices, const std::vector<uint32_t>& indices,
		           const std::vector<RendereableTexture>& textures);
		~Renderable() override = default;

		/* Methods */
		void Bind() override;
		void UnBind() override;

		void ConstructRenderable();

		const Ref<VertexArrayBuffer>& GetVertexArrayBuffer() const;
		const Ref<VertexObjectBuffer>& GetVertexObjectBuffer() const;
		const Ref<IndexBuffer>& GetIndexBuffer() const;
		const std::vector<RenderableVertex>& GetVertices() const;
		const std::vector<uint32_t>& GetIndices() const;
		const std::vector<RendereableTexture> GetTextures() const { return m_RendereableTextures; }

		/* Instantiate */
		static Ref<Renderable> Create(const std::vector<RenderableVertex>& vertices,
		                              const std::vector<uint32_t>& indices,
		                              const std::vector<RendereableTexture>& textures);

	private:
		Ref<VertexArrayBuffer> m_VAO;
		Ref<IndexBuffer> m_IBO;
		Ref<VertexObjectBuffer> m_VBO;
		std::vector<RendereableTexture> m_RendereableTextures;
		std::vector<RenderableVertex> m_Vertices;
		std::vector<uint32_t> m_Indices;
	};
}
