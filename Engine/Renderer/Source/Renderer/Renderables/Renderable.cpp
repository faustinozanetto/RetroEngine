#include "pch.h"
#include "Renderable.h"

namespace Retro::Renderer
{
    Renderable::Renderable(const std::vector<RenderableVertex>& vertices, const std::vector<uint32_t>& indices,
                           const std::vector<RendereableTexture>& textures)
    {
        m_Vertices = vertices;
        m_Indices = indices;
        m_RendereableTextures = textures;
        ConstructRenderable();
    }

    void Renderable::Bind()
    {
        m_VAO->Bind();
    }

    void Renderable::UnBind()
    {
        m_VAO->UnBind();
    }

    void Renderable::ConstructRenderable()
    {
        // Create VBO, VAO and IBO
        m_VAO = VertexArrayBuffer::Create();
        m_VBO = VertexObjectBuffer::Create(reinterpret_cast<float*>(&m_Vertices[0]),
                                           sizeof(RenderableVertex) * m_Vertices.size());
        m_IBO = IndexBuffer::Create((m_Indices).data(), m_Indices.size());
        // Setup VBO Layout
        m_VAO->Bind();
        m_VBO->SetVBOLayout({
            {FloatVec3, "aPos"},
            {FloatVec2, "aTexCoord"},
            {FloatVec3, "aNormal"},
            {FloatVec3, "aTangent"},
            {FloatVec3, "aBiTangent"},
        });
        m_VAO->AddVertexObjectBuffer(m_VBO);
        m_VAO->SetIndexBuffer(m_IBO);
    }

    const Ref<VertexArrayBuffer>& Renderable::GetVertexArrayBuffer() const
    {
        return m_VAO;
    }

    const Ref<VertexObjectBuffer>& Renderable::GetVertexObjectBuffer() const
    {
        return m_VBO;
    }

    const Ref<IndexBuffer>& Renderable::GetIndexBuffer() const
    {
        return m_IBO;
    }

    const std::vector<RenderableVertex>& Renderable::GetVertices() const
    {
        return m_Vertices;
    }

    const std::vector<uint32_t>& Renderable::GetIndices() const
    {
        return m_Indices;
    }

    Ref<Renderable> Renderable::Create(const std::vector<RenderableVertex>& vertices,
                                       const std::vector<uint32_t>& indices,
                                       const std::vector<RendereableTexture>& textures)
    {
        return CreateRef<Renderable>(vertices, indices, textures);
    }
}
