#include "pch.h"

#include "OpenGLTexture.cpp"
#include <stb_image.h>
#include "OpenGLTextureCubemap.h"


#include "glm/ext/matrix_clip_space.hpp"
#include "glm/ext/matrix_transform.hpp"


namespace Retro::Renderer
{
	OpenGLTextureCubemap::OpenGLTextureCubemap(const FTextureSpecification& textureSpecification) : Asset(AssetType::Texture)
	{
		Logger::Line();
		m_EquirectangularShader = Shader::Create("Assets/Shaders/Skybox/Equirectangular.vert", 
			"Assets/Shaders/Skybox/Equirectangular.frag");
		m_TextureSpecification = textureSpecification;
		Logger::Info("OpenGLTextureCubemap::OpenGLTextureCubemap | Loading cubemap: ");
		Logger::Info("  - Path: " + m_TextureSpecification.path);
		Logger::Info("  - Filtering: " + Texture::GetTextureFilteringToString(m_TextureSpecification.filtering));
		Logger::Info("  - Wrapping: " + Texture::GetTextureWrappingToString(m_TextureSpecification.wrapping));

		const uint32_t cubemapSize = 2048;
		const uint32_t irradianceMapSize = 32;
		const uint32_t radianceMapSize = cubemapSize / 4;

		// Variables for stb.
		int width, height, channels;
		float* data = nullptr;

		// Load file using STB.
		stbi_set_flip_vertically_on_load(1);
		{
			data = stbi_loadf(m_TextureSpecification.path.c_str(), &width, &height, &channels, 0);
		}

		RETRO_CORE_ASSERT(data, "Failed to load data from image");

		// Updating size.
		m_Width = width;
		m_Height = height;
		m_Channels = channels;

		Logger::Info(
			"  - Width: " + std::to_string(m_Width) + " Height: " + std::to_string(
				m_Height));
		Logger::Info("  - Channels: " + std::to_string(m_Channels));

		const GLenum internalFormat = GL_RGB16F, dataFormat = GL_RGB;

		m_InternalFormat = internalFormat;
		m_DataFormat = dataFormat;

		Logger::Info("Internal format: " + ConvertTextureEnumToString(m_InternalFormat));
		Logger::Info("Data format: " + ConvertTextureEnumToString(m_DataFormat));
		SetupCube();
		glm::mat4 captureProjection = glm::perspective(glm::radians(90.0f), 1.0f, 0.1f, 10.0f);
		glm::mat4 captureViews[] =
		{
			lookAt(glm::vec3(0.0f, 0.0f, 0.0f), glm::vec3(1.0f, 0.0f, 0.0f), glm::vec3(0.0f, -1.0f, 0.0f)),
			lookAt(glm::vec3(0.0f, 0.0f, 0.0f), glm::vec3(-1.0f, 0.0f, 0.0f), glm::vec3(0.0f, -1.0f, 0.0f)),
			lookAt(glm::vec3(0.0f, 0.0f, 0.0f), glm::vec3(0.0f, 1.0f, 0.0f), glm::vec3(0.0f, 0.0f, 1.0f)),
			lookAt(glm::vec3(0.0f, 0.0f, 0.0f), glm::vec3(0.0f, -1.0f, 0.0f), glm::vec3(0.0f, 0.0f, -1.0f)),
			lookAt(glm::vec3(0.0f, 0.0f, 0.0f), glm::vec3(0.0f, 0.0f, 1.0f), glm::vec3(0.0f, -1.0f, 0.0f)),
			lookAt(glm::vec3(0.0f, 0.0f, 0.0f), glm::vec3(0.0f, 0.0f, -1.0f), glm::vec3(0.0f, -1.0f, 0.0f))
		};


		glCreateTextures(GL_TEXTURE_2D, 1, &m_ObjectHandle);
		glBindTexture(GL_TEXTURE_2D, m_ObjectHandle);

		glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB16F, width, height, 0, GL_RGB, GL_FLOAT, data);

		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
		stbi_image_free(data);

		unsigned int captureFBO;
		unsigned int captureRBO;
		glGenFramebuffers(1, &captureFBO);
		glGenRenderbuffers(1, &captureRBO);

		glBindFramebuffer(GL_FRAMEBUFFER, captureFBO);
		glBindRenderbuffer(GL_RENDERBUFFER, captureRBO);
		glRenderbufferStorage(GL_RENDERBUFFER, GL_DEPTH_COMPONENT24, 2048, 2048);
		glFramebufferRenderbuffer(GL_FRAMEBUFFER, GL_DEPTH_ATTACHMENT, GL_RENDERBUFFER, captureRBO);

		glGenTextures(1, &envCubemap);
		glBindTexture(GL_TEXTURE_CUBE_MAP, envCubemap);
		for (unsigned int i = 0; i < 6; ++i)
		{
			glTexImage2D(GL_TEXTURE_CUBE_MAP_POSITIVE_X + i, 0, GL_RGB16F, 2048, 2048, 0, GL_RGB, GL_FLOAT, nullptr);
		}
		glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
		glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
		glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_R, GL_CLAMP_TO_EDGE);
		glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);
		glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

		m_EquirectangularShader->Bind();
		m_EquirectangularShader->SetMat4("camera.u_ProjectionMatrix", captureProjection);

		Bind(0);

		glViewport(0, 0, 2048, 2048); // don't forget to configure the viewport to the capture dimensions.
		glBindFramebuffer(GL_FRAMEBUFFER, captureFBO);
		for (unsigned int i = 0; i < 6; ++i)
		{
			m_EquirectangularShader->SetMat4("camera.u_ViewMatrix", captureViews[i]);
			glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_TEXTURE_CUBE_MAP_POSITIVE_X + i, envCubemap,
				0);
			glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
			RenderCube();
		}
		glBindFramebuffer(GL_FRAMEBUFFER, 0);

		// pbr: create a pre-filter cube map, and re-scale capture FBO to pre-filter scale.
// --------------------------------------------------------------------------------
		glGenTextures(1, &prefilterMap);
		glBindTexture(GL_TEXTURE_CUBE_MAP, prefilterMap);
		for (unsigned int i = 0; i < 6; ++i)
		{
			glTexImage2D(GL_TEXTURE_CUBE_MAP_POSITIVE_X + i, 0, GL_RGB16F, 512, 512, 0, GL_RGB, GL_FLOAT, nullptr);
		}
		glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
		glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
		glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_R, GL_CLAMP_TO_EDGE);
		glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);
		// be sure to set minification filter to mip_linear 
		glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
		glGenerateMipmap(GL_TEXTURE_CUBE_MAP);

		// ----------------------------------------------------------------------------------------------------
		/*
		glBindFramebuffer(GL_FRAMEBUFFER, captureFBO);
		unsigned int maxMipLevels = 5;
		for (unsigned int mip = 0; mip < maxMipLevels; ++mip)
		{
			// resize framebuffer according to mip-level size.
			auto mipWidth = 512 * std::pow(0.5, mip);
			auto mipHeight = 512 * std::pow(0.5, mip);
			glBindRenderbuffer(GL_RENDERBUFFER, captureRBO);
			glRenderbufferStorage(GL_RENDERBUFFER, GL_DEPTH_COMPONENT24, mipWidth, mipHeight);
			glViewport(0, 0, mipWidth, mipHeight);

			float roughness = static_cast<float>(mip) / static_cast<float>(maxMipLevels - 1);
			m_PrefilterShader->SetFloat("parameters.roughness", roughness);
			for (unsigned int i = 0; i < 6; ++i)
			{
				m_PrefilterShader->SetMat4("camera.view", captureViews[i]);
				glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_TEXTURE_CUBE_MAP_POSITIVE_X + i,
					prefilterMap, mip);

				glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
				RenderCube();
			}
		}
		glBindFramebuffer(GL_FRAMEBUFFER, 0);
		*/

		glBindTexture(GL_TEXTURE_CUBE_MAP, envCubemap);
		glGenerateMipmap(GL_TEXTURE_CUBE_MAP);
	}

	OpenGLTextureCubemap::~OpenGLTextureCubemap()
	{
		glDeleteTextures(1, &m_ObjectHandle);
	}

	void OpenGLTextureCubemap::Bind()
	{
		Bind(0);
	}

	void OpenGLTextureCubemap::Bind(int slot)
	{
		glBindTextureUnit(slot, m_ObjectHandle);
	}

	void OpenGLTextureCubemap::UnBind()
	{
		glBindTexture(m_ObjectHandle, 0);
	}

	void OpenGLTextureCubemap::SetupCube()
	{
		float vertices[] = {
			// back face
			-1.0f, -1.0f, -1.0f, 0.0f, 0.0f, -1.0f, 0.0f, 0.0f, // bottom-left
			1.0f, 1.0f, -1.0f, 0.0f, 0.0f, -1.0f, 1.0f, 1.0f, // top-right
			1.0f, -1.0f, -1.0f, 0.0f, 0.0f, -1.0f, 1.0f, 0.0f, // bottom-right         
			1.0f, 1.0f, -1.0f, 0.0f, 0.0f, -1.0f, 1.0f, 1.0f, // top-right
			-1.0f, -1.0f, -1.0f, 0.0f, 0.0f, -1.0f, 0.0f, 0.0f, // bottom-left
			-1.0f, 1.0f, -1.0f, 0.0f, 0.0f, -1.0f, 0.0f, 1.0f, // top-left
			// front face
			-1.0f, -1.0f, 1.0f, 0.0f, 0.0f, 1.0f, 0.0f, 0.0f, // bottom-left
			1.0f, -1.0f, 1.0f, 0.0f, 0.0f, 1.0f, 1.0f, 0.0f, // bottom-right
			1.0f, 1.0f, 1.0f, 0.0f, 0.0f, 1.0f, 1.0f, 1.0f, // top-right
			1.0f, 1.0f, 1.0f, 0.0f, 0.0f, 1.0f, 1.0f, 1.0f, // top-right
			-1.0f, 1.0f, 1.0f, 0.0f, 0.0f, 1.0f, 0.0f, 1.0f, // top-left
			-1.0f, -1.0f, 1.0f, 0.0f, 0.0f, 1.0f, 0.0f, 0.0f, // bottom-left
			// left face
			-1.0f, 1.0f, 1.0f, -1.0f, 0.0f, 0.0f, 1.0f, 0.0f, // top-right
			-1.0f, 1.0f, -1.0f, -1.0f, 0.0f, 0.0f, 1.0f, 1.0f, // top-left
			-1.0f, -1.0f, -1.0f, -1.0f, 0.0f, 0.0f, 0.0f, 1.0f, // bottom-left
			-1.0f, -1.0f, -1.0f, -1.0f, 0.0f, 0.0f, 0.0f, 1.0f, // bottom-left
			-1.0f, -1.0f, 1.0f, -1.0f, 0.0f, 0.0f, 0.0f, 0.0f, // bottom-right
			-1.0f, 1.0f, 1.0f, -1.0f, 0.0f, 0.0f, 1.0f, 0.0f, // top-right
			// right face
			1.0f, 1.0f, 1.0f, 1.0f, 0.0f, 0.0f, 1.0f, 0.0f, // top-left
			1.0f, -1.0f, -1.0f, 1.0f, 0.0f, 0.0f, 0.0f, 1.0f, // bottom-right
			1.0f, 1.0f, -1.0f, 1.0f, 0.0f, 0.0f, 1.0f, 1.0f, // top-right         
			1.0f, -1.0f, -1.0f, 1.0f, 0.0f, 0.0f, 0.0f, 1.0f, // bottom-right
			1.0f, 1.0f, 1.0f, 1.0f, 0.0f, 0.0f, 1.0f, 0.0f, // top-left
			1.0f, -1.0f, 1.0f, 1.0f, 0.0f, 0.0f, 0.0f, 0.0f, // bottom-left     
			// bottom face
			-1.0f, -1.0f, -1.0f, 0.0f, -1.0f, 0.0f, 0.0f, 1.0f, // top-right
			1.0f, -1.0f, -1.0f, 0.0f, -1.0f, 0.0f, 1.0f, 1.0f, // top-left
			1.0f, -1.0f, 1.0f, 0.0f, -1.0f, 0.0f, 1.0f, 0.0f, // bottom-left
			1.0f, -1.0f, 1.0f, 0.0f, -1.0f, 0.0f, 1.0f, 0.0f, // bottom-left
			-1.0f, -1.0f, 1.0f, 0.0f, -1.0f, 0.0f, 0.0f, 0.0f, // bottom-right
			-1.0f, -1.0f, -1.0f, 0.0f, -1.0f, 0.0f, 0.0f, 1.0f, // top-right
			// top face
			-1.0f, 1.0f, -1.0f, 0.0f, 1.0f, 0.0f, 0.0f, 1.0f, // top-left
			1.0f, 1.0f, 1.0f, 0.0f, 1.0f, 0.0f, 1.0f, 0.0f, // bottom-right
			1.0f, 1.0f, -1.0f, 0.0f, 1.0f, 0.0f, 1.0f, 1.0f, // top-right     
			1.0f, 1.0f, 1.0f, 0.0f, 1.0f, 0.0f, 1.0f, 0.0f, // bottom-right
			-1.0f, 1.0f, -1.0f, 0.0f, 1.0f, 0.0f, 0.0f, 1.0f, // top-left
			-1.0f, 1.0f, 1.0f, 0.0f, 1.0f, 0.0f, 0.0f, 0.0f // bottom-left        
		};
		m_CubeVAO = VertexArrayBuffer::Create();
		Shared<VertexObjectBuffer> cubeVBO = VertexObjectBuffer::Create(vertices, sizeof(vertices));

		cubeVBO->SetVBOLayout({
			{VBOElementType::FloatVec3, "aPos"},
			{VBOElementType::FloatVec2, "aNormal"},
			{VBOElementType::FloatVec3, "aTexCoord"},
			});
		m_CubeVAO->AddVertexObjectBuffer(cubeVBO);
	}

	void OpenGLTextureCubemap::RenderCube()
	{
		m_CubeVAO->Bind();
		glDrawArrays(GL_TRIANGLES, 0, 36);
	}
}
