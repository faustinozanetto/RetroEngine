#include "pch.h"

#include "RetroApplication.h"
#include "Logger/Logger.h"
#include "Renderer/Renderer/Renderer.h"

namespace Retro {
	RetroApplication* RetroApplication::s_Instance = nullptr;

	RetroApplication::RetroApplication(const FRetroApplicationSpecification& applicationSpecification) {
		// Update variables.
		s_Instance = this;
		m_ApplicationSpecification = applicationSpecification;
		// Initialize Logger.
		Logger::Initialize();
		Logger::Line();
		// Create window.
		FWindowSpecification windowSpecification = FWindowSpecification();
		m_Window = Window::Create(windowSpecification);
		// Initialize Renderer
		Renderer::Initialize(RenderingAPIType::OpenGL);
	}

	RetroApplication::~RetroApplication() = default;

	void RetroApplication::RunApplication()
	{
		while (true) {
			m_Window->WindowLoop();
		}
	}
}