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

		void Bind() override;
		void UnBind() override;
	};
}
