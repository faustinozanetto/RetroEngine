#pragma once

#include "Renderer/Window/Window.h"

#include "glad/glad.h"
#include "GLFW/glfw3.h"

namespace Retro::Renderer {
	class OpenGLWindow : public Window {
	public:
		/* Constructor & Destructor */
		OpenGLWindow() = default;
		OpenGLWindow(const FWindowSpecification& specification);
		~OpenGLWindow() override;

		/* Getters */
		void* GetNativeWindow() const override;

		/* Methods */
		void SetEnableVSync(bool useVSync) override;
		void SetWindowTitle(const std::string& title) override;
		void SetupWindowCallbacks() override;

	private:
		bool InitializeWindow();

	private:
		GLFWwindow* m_OpenGLWindow{};
	};

}