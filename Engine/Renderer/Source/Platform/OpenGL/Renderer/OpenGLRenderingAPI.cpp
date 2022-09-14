#include "pch.h"

#include "Platform/OpenGL/Renderer/OpenGLRenderingAPI.h"
#include <glad/glad.h>
#include <GLFW/glfw3.h>

namespace Retro::Renderer {
	OpenGLRenderingAPI::OpenGLRenderingAPI() {
		Logger::Info("OpenGLRenderingAPI::OpenGLRenderingAPI | Created rendering API.");
	}

	OpenGLRenderingAPI::~OpenGLRenderingAPI() = default;

	bool OpenGLRenderingAPI::Initialize()
	{
		Logger::Info("OpenGLRenderingAPI::Initialize | Initializing rendering API.");
		// Initialize OpenGL configuration.
		glEnable(GL_BLEND);
		glFrontFace(GL_CW);
		glEnable(GL_TEXTURE_CUBE_MAP_SEAMLESS);
		glEnable(GL_DEPTH_TEST);
		glDepthFunc(GL_LESS);
		glEnable(GL_LINE_SMOOTH);
		return true;
	}

	void OpenGLRenderingAPI::SetClearColor(glm::vec4 color)
	{
		glClearColor(color.r, color.g, color.b, color.a);
	}

	void OpenGLRenderingAPI::ClearScreen()
	{
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	}
}