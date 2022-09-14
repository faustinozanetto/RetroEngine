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
		virtual void AddVertexObjectBuffer(const Ref<VertexObjectBuffer>& vbo) = 0;
		virtual const std::vector<Ref<VertexObjectBuffer>>& GetVertexObjectBuffers() const = 0;
		virtual const Ref<IndexBuffer>& GetIndexBuffer() const = 0;
		virtual void SetIndexBuffer(const Ref<IndexBuffer>& ibo) = 0;

		/* Instantiate */
		static Ref<VertexArrayBuffer> Create();
	};
}
