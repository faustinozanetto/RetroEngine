#include "pch.h"

#include "open_gl_renderer_api.h"

#include "core/base.h"
#include "core/assert.h"
#include <glad/glad.h>
#include <GLFW/glfw3.h>

namespace retro::renderer
{
	static std::string GetStringForType(GLenum type)
	{
		switch (type)
		{
		case GL_DEBUG_TYPE_ERROR:
			return "Error";
		case GL_DEBUG_TYPE_DEPRECATED_BEHAVIOR:
			return "Deprecated behavior";
		case GL_DEBUG_TYPE_UNDEFINED_BEHAVIOR:
			return "Undefined behavior";
		case GL_DEBUG_TYPE_PORTABILITY:
			return "Portability issue";
		case GL_DEBUG_TYPE_PERFORMANCE:
			return "Performance issue";
		case GL_DEBUG_TYPE_MARKER:
			return "Stream annotation";
		default:
			return "";
		}
	}

	static std::string GetStringForSource(GLenum source)
	{
		switch (source)
		{
		case GL_DEBUG_SOURCE_API:
			return "API";
		case GL_DEBUG_SOURCE_WINDOW_SYSTEM:
			return "Window System";
		case GL_DEBUG_SOURCE_SHADER_COMPILER:
			return "Shader compiler";
		case GL_DEBUG_SOURCE_THIRD_PARTY:
			return "Third party";
		case GL_DEBUG_SOURCE_APPLICATION:
			return "Application";
		case GL_DEBUG_SOURCE_OTHER:
			return "Other";
		default:
			return "";
		}
	}

	static std::string GetStringForSeverity(GLenum severity)
	{
		switch (severity)
		{
		case GL_DEBUG_SEVERITY_HIGH:
			return "High";
		case GL_DEBUG_SEVERITY_MEDIUM:
			return "Medium";
		case GL_DEBUG_SEVERITY_LOW:
			return "Low";
		case GL_DEBUG_SEVERITY_NOTIFICATION:
			return "Notification";
		case GL_DEBUG_SOURCE_API:
			return "Source API";
		default:
			return ("");
		}
	}

	void OpenGLMessageCallback(
			unsigned source,
			unsigned type,
			unsigned id,
			unsigned severity,
			int length,
			const char *message,
			const void *userParam)
	{
		logger::error("Message: " + std::string(message));
		logger::error("Type: " + GetStringForType(type));
		logger::error("Source: " + GetStringForSource(source));
		logger::error("Severity: " + GetStringForSeverity(severity));
		logger::error("ID: " + id);
	}

	open_gl_renderer_api::open_gl_renderer_api()
	{
		logger::info("open_gl_renderer_api::open_gl_renderer_api | Created rendering API.");
	}

	open_gl_renderer_api::~open_gl_renderer_api() = default;

	bool open_gl_renderer_api::initialize()
	{
		logger::info("open_gl_renderer_api::initialize | Initializing rendering API.");
		glEnable(GL_DEBUG_OUTPUT);
		glEnable(GL_DEBUG_OUTPUT_SYNCHRONOUS);
		glDebugMessageCallback(OpenGLMessageCallback, nullptr);
		glDebugMessageControl(GL_DONT_CARE, GL_DONT_CARE, GL_DEBUG_SEVERITY_NOTIFICATION, 0, nullptr, GL_FALSE);

		// OpenGL.
		glEnable(GL_BLEND);
		glEnable(GL_TEXTURE_CUBE_MAP_SEAMLESS);
		glEnable(GL_DEPTH_TEST);
		return true;
	}

	void open_gl_renderer_api::set_clear_color(glm::vec4 color)
	{
		glClearColor(color.r, color.g, color.b, color.a);
	}

	void open_gl_renderer_api::set_renderer_mode(renderer_mode renderer_mode)
	{
		switch (renderer_mode)
		{
		case renderer_mode::normal:
			glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
			break;
		case renderer_mode::wireframe:
			glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
			break;
		case renderer_mode::point:
			glPolygonMode(GL_FRONT_AND_BACK, GL_POINT);
			break;
		}
	}

	void open_gl_renderer_api::set_renderer_state(renderer_state renderer_state, bool enabled)
	{
		if (enabled)
			glEnable(get_renderer_state_to_open_gl(renderer_state));
		else
			glDisable(get_renderer_state_to_open_gl(renderer_state));
	}

	void open_gl_renderer_api::clear_screen()
	{
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	}

	void open_gl_renderer_api::process_rendereable(int size)
	{
		glDrawElements(GL_TRIANGLES, size, GL_UNSIGNED_INT, nullptr);
		// glBindTexture(GL_TEXTURE_2D, 0);
	}

	void open_gl_renderer_api::bind_texture(uint32_t texture_handle, uint32_t texture_slot)
	{
		glBindTextureUnit(texture_slot, texture_handle);
	}

	double open_gl_renderer_api::get_time()
	{
		return glfwGetTime();
	}

	GLenum open_gl_renderer_api::get_renderer_state_to_open_gl(renderer_state renderer_state)
	{
		switch (renderer_state)
		{
		case renderer_state::depth_test:
			return GL_DEPTH_TEST;
		}
		RETRO_CORE_ASSERT(false, "open_gl_renderer_api::get_renderer_state_to_open_gl | Unknown render state");
		return 0;
	}
}
