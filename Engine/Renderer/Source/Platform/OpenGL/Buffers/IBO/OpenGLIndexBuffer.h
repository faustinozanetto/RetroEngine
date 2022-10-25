#pragma once

#include "Renderer/Buffers/IBO/IndexBuffer.h"

namespace Retro::Renderer
{
	class OpenGLIndexBuffer : public IndexBuffer
	{
	public:
		/* Constructor & Destructor */
		OpenGLIndexBuffer(uint32_t* data, uint32_t size);
		~OpenGLIndexBuffer() override;

		/* Methods */
		void Bind() override;
		void UnBind() override;

		uint32_t GetSize() const override;

	private:
		uint32_t m_Size;
	};
}
