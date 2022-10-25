#include "pch.h"

#include "InterfaceImplementation.h"

#include "Platform/OpenGL/Interfaces/OpenGLInterfaceImplementation.h"
#include "Renderer/Renderer/RendererContext.h"
#include "Renderer/Renderer/Renderer.h"

namespace Retro::Renderer
{
	Unique<InterfaceImplementation> InterfaceImplementation::Create()
	{
		switch (Renderer::GetRenderingAPIType())
		{
		case RenderingAPIType::None:
			{
				Logger::Error("InterfaceImplementation::Create | Unknown rendering api!.");
				return nullptr;
			}
		case RenderingAPIType::OpenGL:
			{
				return CreateUnique<OpenGLInterfaceImplementation>();
			}
		}
		return {};
	}
}
