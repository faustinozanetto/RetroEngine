#pragma once

#include "Core/Base.h"
#include "Renderer/Buffers/IBO/IndexBuffer.h"
#include "Renderer/Buffers/VBO/VertexObjectBuffer.h"
#include "Renderer/Renderer/GraphicsObject.h"

namespace Retro::Renderer
{
	class VertexArrayBuffer : public GraphicsObject
	{
	public:
		/* Destructor */
		VertexArrayBuffer();
		~VertexArrayBuffer() override;

		/* Methods */
		virtual void AddVertexObjectBuffer(const Shared<VertexObjectBuffer>& vbo) = 0;
		virtual const std::vector<Shared<VertexObjectBuffer>>& GetVertexObjectBuffers() const = 0;
		virtual const Shared<IndexBuffer>& GetIndexBuffer() const = 0;
		virtual void SetIndexBuffer(const Shared<IndexBuffer>& ibo) = 0;

		/* Instantiate */
		static Shared<VertexArrayBuffer> Create();
	};
}
