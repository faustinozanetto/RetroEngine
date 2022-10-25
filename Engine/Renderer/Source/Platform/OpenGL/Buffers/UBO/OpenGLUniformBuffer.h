#pragma once
#include "Renderer/Buffers/UBO/UniformBuffer.h"

namespace Retro::Renderer
{
	class OpenGLUniformBuffer : public UniformBuffer
	{
	public:
		/* Constructor & Destructor */
		OpenGLUniformBuffer(uint32_t size, uint32_t binding);
		~OpenGLUniformBuffer() override;

		/* Methods */
		void Bind() override;
		void UnBind() override;
		void SetData(const void* data, uint32_t size, uint32_t offset) override;
		void SetIBOLayout(const VBOLayout& layout, uint32_t block, uint32_t count = 1) override;
	private:
		VBOLayout m_Layout;
	};
}
