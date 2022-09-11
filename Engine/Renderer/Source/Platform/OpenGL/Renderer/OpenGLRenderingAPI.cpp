#include "pch.h"

#include "Platform/OpenGL/Renderer/OpenGLRenderingAPI.h"
#include <glad/glad.h>

namespace Retro {
	OpenGLRenderingAPI::OpenGLRenderingAPI() {

	}

	OpenGLRenderingAPI::~OpenGLRenderingAPI() = default;

	bool OpenGLRenderingAPI::Initialize()
	{
		// Initialize OpenGL configuration.
		glEnable(GL_BLEND);
		glFrontFace(GL_CW);
		glEnable(GL_TEXTURE_CUBE_MAP_SEAMLESS);
		glEnable(GL_DEPTH_TEST);
		glDepthFunc(GL_LESS);
		glEnable(GL_LINE_SMOOTH);

		return true;
	}
}