#include "pch.h"

#include "OpenGLWindow.h"
#include "Renderer/Renderer/Renderer.h"

namespace Retro::Renderer
{
    OpenGLWindow::OpenGLWindow(const FWindowSpecification& specification)
    {
        Logger::Line();
        // Update window specification.
        m_WindowSpecification.windowTitle = specification.windowTitle;
        m_WindowSpecification.width = specification.width;
        m_WindowSpecification.height = specification.height;
        m_WindowSpecification.vSync = specification.vSync;
        // Initialize.
        if (!InitializeWindow())
        {
            Logger::Error("Initialized | Could not initialize window.");
        }
    }

    OpenGLWindow::~OpenGLWindow()
    {
        glfwDestroyWindow(m_OpenGLWindow);
        glfwTerminate();
    }

    bool OpenGLWindow::InitializeWindow()
    {
        // Initializing GLFW
        if (glfwInit() == GLFW_FALSE)
        {
            return false;
        }
        Logger::Info("Initialization | GLFW Success");

        // Create OpenGL Window.
        Logger::Info("Initialization | Creating OpenGL Window");
        Logger::Info("Window Title: " + m_WindowSpecification.windowTitle);
        Logger::Info("Window Width: " + std::to_string(m_WindowSpecification.width));
        Logger::Info("Window Height: " + std::to_string(m_WindowSpecification.height));
        const auto vSyncEnabledLog = m_WindowSpecification.vSync ? "Enabled" : "Disabled";
        Logger::Info("Window VSync: " + std::to_string(*vSyncEnabledLog));
        Logger::Line();

        // Setup window hints
        glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 4);
        glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 6);
        glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
        glfwWindowHint(GLFW_OPENGL_DEBUG_CONTEXT, GLFW_TRUE);
        glfwWindowHint(GLFW_SAMPLES, 8);

        // Creating the window
        m_OpenGLWindow = glfwCreateWindow(m_WindowSpecification.width,
                                          m_WindowSpecification.height, m_WindowSpecification.windowTitle.c_str(),
                                          nullptr,
                                          nullptr);
        RETRO_ASSERT(m_OpenGLWindow, "Initialization | Failed to create OpenGL Window.");

        // Set user pointer, used in callbacks.
        glfwSetWindowUserPointer(m_OpenGLWindow, &m_WindowSpecification);

        // Handle Initial VSync.
        SetEnableVSync(true);

        // Setup callbacks.
        SetupWindowCallbacks();
        Logger::Line();
        return true;
    }

    void* OpenGLWindow::GetNativeWindow() const
    {
        return m_OpenGLWindow;
    }

    void OpenGLWindow::SetEnableVSync(bool useVSync)
    {
        // OpenGL Vsync implementation.
        glfwSwapInterval(useVSync ? 1 : 0);
        m_WindowSpecification.vSync = useVSync;
    }

    void OpenGLWindow::SetWindowTitle(const std::string& title)
    {
        glfwSetWindowTitle(m_OpenGLWindow, title.c_str());
        m_WindowSpecification.windowTitle = title;
    }

    void OpenGLWindow::SetupWindowCallbacks()
    {
        glfwSetWindowSizeCallback(m_OpenGLWindow, [](GLFWwindow* window, int width, int height)
        {
            FWindowSpecification& windowSpecification = *static_cast<FWindowSpecification*>(
                glfwGetWindowUserPointer(window));
            windowSpecification.width = width;
            windowSpecification.height = height;

            std::stringstream ss;
            ss << "WindowResizeEvent: " << width << ", " << height;
            Renderer::SetViewport(0, 0, width, height);
            Logger::Info(ss.str());
        });
    }
}
