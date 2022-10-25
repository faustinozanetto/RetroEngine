#include "pch.h"

#include "Renderer/Shader/Shader.h"
#include "Renderer/Renderer/RendererContext.h"
#include "Renderer/Renderer/Renderer.h"
#include "Platform/OpenGL/Shader/OpenGLShader.h"

namespace Retro::Renderer
{
	Ref<Shader> Shader::Create(const std::string& vertexPath, const std::string& fragmentPath)
	{
		switch (Renderer::GetRenderingAPIType())
		{
		case RenderingAPIType::None:
			{
				Logger::Error("Shader::Create | Unknown rendering api!.");
				return nullptr;
			}
		case RenderingAPIType::OpenGL:
			{
				return CreateRef<OpenGLShader>(vertexPath, fragmentPath);
			}
		}
		return {};
	}
}
