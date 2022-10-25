#pragma once

#include "Renderer/Buffers/VBO/VertexObjectBuffer.h"

namespace Retro::Renderer
{
	class OpenGLVertexObjectBuffer : public VertexObjectBuffer
	{
	public:
		/* Constructor & Destructor */
		OpenGLVertexObjectBuffer(const float* vertices, uint32_t size);
		~OpenGLVertexObjectBuffer() override;

		/* Methods */
		void Bind() override;
		void UnBind() override;

		const VBOLayout& GetVBOLayout() const override;
		void SetVBOLayout(const VBOLayout& layout) override;

		static uint32_t GetOpenGLVBOElementType(VBOElementType vboElementType);
	private:
		VBOLayout m_VBOLayout;
	};
}
