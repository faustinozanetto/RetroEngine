#pragma once

#include "Core/Base.h"
#include "Renderer/Textures/Texture.h"
#include "Renderer/Renderer/GraphicsObject.h"

namespace Retro::Renderer
{
	class TextureCubemap : public GraphicsObject
	{
	public:
		/* Destructor */
		~TextureCubemap() override = default;

		/* Methods */
		void Bind() override = 0;
		virtual void Bind(int slot) = 0;
		void UnBind() override = 0;

		static Shared<TextureCubemap> Create(const FTextureSpecification& textureSpecification);
	};
}
