#pragma once

#include "Renderer/Buffers/IBO/IndexBuffer.h"
#include "Renderer/Buffers/VAO/VertexArrayBuffer.h"

namespace Retro::Renderer
{
	class OpenGLVertexArrayBuffer : public VertexArrayBuffer
	{
	public:
		/* Constructor & Destructor */
		OpenGLVertexArrayBuffer();
		~OpenGLVertexArrayBuffer() override;

		/* Methods */
		void Bind() override;
		void UnBind() override;

		void AddVertexObjectBuffer(const Shared<VertexObjectBuffer>& vbo) override;
		const std::vector<Shared<VertexObjectBuffer>>& GetVertexObjectBuffers() const override;
		const Shared<IndexBuffer>& GetIndexBuffer() const override;
		void SetIndexBuffer(const Shared<IndexBuffer>& ibo) override;

	private:
		std::vector<Shared<VertexObjectBuffer>> m_VBOs;
		Shared<IndexBuffer> m_IBO;
	};
}
