#pragma once

#include "Core/Base.h"
#include "Renderer/Renderer/GraphicsObject.h"

namespace Retro::Renderer
{
	class IndexBuffer : public GraphicsObject
	{
	public:
		/* Destructor */
		~IndexBuffer() override;

		/* Methods */
		virtual uint32_t GetSize() const = 0;

		/* Instantiate */
		static Ref<IndexBuffer> Create(uint32_t* data, uint32_t size);
	};
}
