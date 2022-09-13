#pragma once

#include "Core/Base.h"
#include "Renderer/Renderer/GraphicsObject.h"

namespace Retro::Renderer
{
	class VertexArrayBuffer : public GraphicsObject
	{
	public:
		/* Destructor */
		VertexArrayBuffer();
		~VertexArrayBuffer() override;

		/* Instantiate */
		static Ref<VertexArrayBuffer> Create();
	};
}
