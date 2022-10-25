#pragma once

#include "Renderer/Textures/TextureCubemap.h"
#include "Core/Assets/Asset.h"
#include "glad/glad.h"
#include "Renderer/Buffers/VAO/VertexArrayBuffer.h"
#include "Renderer/Shader/Shader.h"

namespace Retro::Renderer
{
	class OpenGLTextureCubemap final : public TextureCubemap, public Asset
	{
	public:
		OpenGLTextureCubemap(const FTextureSpecification& textureSpecification);
		~OpenGLTextureCubemap() override;

		/* Methods */
		void Bind() override;
		void Bind(int slot) override;
		void UnBind() override;

		void SetupCube();
		void RenderCube();

	protected:
		FTextureSpecification m_TextureSpecification;
		Shared<Shader> m_EquirectangularShader;
		int m_MipMapLevels;
		int m_Channels;
		int m_Width;
		int m_Height;
		GLenum m_InternalFormat;
		GLenum m_DataFormat;

		Shared<VertexArrayBuffer> m_CubeVAO, m_QuadVAO;

		unsigned int envCubemap;
		unsigned int brdfLUTTexture;
		unsigned int prefilterMap;
		unsigned int irradianceMap;
	};
}
