#pragma once

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
		
		void AddVertexObjectBuffer(const Ref<VertexObjectBuffer>& vbo) override;
		const std::vector<Ref<VertexObjectBuffer>>& GetVertexObjectBuffers() const override;
	private:
		std::vector<Ref<VertexObjectBuffer>> m_VBOs;
	};
}
