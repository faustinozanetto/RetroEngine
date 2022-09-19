#include "pch.h"

#include "Platform/OpenGL/Renderer/OpenGLRenderingAPI.h"
#include <glad/glad.h>
#include <GLFW/glfw3.h>

namespace Retro::Renderer
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
        const char* message,
        const void* userParam)
    {
        Logger::Error("Message: " + std::string(message));
        Logger::Error("Type: " + GetStringForType(type));
        Logger::Error("Source: " + GetStringForSource(source));
        Logger::Error("Severity: " + GetStringForSeverity(severity));
        Logger::Error("ID: " + id);
    }

    OpenGLRenderingAPI::OpenGLRenderingAPI()
    {
        Logger::Info("OpenGLRenderingAPI::OpenGLRenderingAPI | Created rendering API.");
    }

    OpenGLRenderingAPI::~OpenGLRenderingAPI() = default;

    bool OpenGLRenderingAPI::Initialize()
    {
        Logger::Info("OpenGLRenderingAPI::Initialize | Initializing rendering API.");
        glEnable(GL_DEBUG_OUTPUT);
        glEnable(GL_DEBUG_OUTPUT_SYNCHRONOUS);
        glDebugMessageCallback(OpenGLMessageCallback, nullptr);
        glDebugMessageControl(GL_DONT_CARE, GL_DONT_CARE, GL_DEBUG_SEVERITY_NOTIFICATION, 0, nullptr, GL_FALSE);

        glEnable(GL_DEPTH_TEST);
        return true;
    }

    void OpenGLRenderingAPI::SetClearColor(glm::vec4 color)
    {
        glClearColor(color.r, color.g, color.b, color.a);
    }

    void OpenGLRenderingAPI::SetRenderMode(ERenderMode renderMode)
    {
        switch (renderMode)
        {
        case ERenderMode::Normal:
            glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
            break;
        case ERenderMode::Wireframe:
            glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
            break;
        case ERenderMode::Point:
            glPolygonMode(GL_FRONT_AND_BACK, GL_POINT);
            break;
        }
    }

    void OpenGLRenderingAPI::ClearScreen()
    {
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    }

    void OpenGLRenderingAPI::ProcessRendereable(int size)
    {
        glDrawElements(GL_TRIANGLES, size, GL_UNSIGNED_INT, nullptr);
    }

    double OpenGLRenderingAPI::GetTime()
    {
        return glfwGetTime();
    }
}
