#pragma once

#include "Core/Base.h"

namespace Retro::Renderer
{
	struct FWindowSpecification
	{
		std::string windowTitle;
		int width;
		int height;
		bool vSync;

		FWindowSpecification(const std::string& windowTitle = "Retro Engine", int width = 1920,
		                     int height = 1080, bool vSync = false) : windowTitle(windowTitle), width(width),
		                                                              height(height), vSync(vSync)
		{
		}
	};

	class Window
	{
	public:
		/* Destructor */
		virtual ~Window() = default;

		/* Methods */
		virtual void SetEnableVSync(bool useVSync) = 0;
		virtual void SetWindowTitle(const std::string& title) = 0;
		virtual void SetupWindowCallbacks() = 0;

		/* Getters */
		const FWindowSpecification& GetWindowSpecification();
		virtual void* GetNativeWindow() const = 0;
		bool IsVSyncEnabled() const;

		/* Instantiate */
		static Unique<Window> Create(const FWindowSpecification& specification = FWindowSpecification());
	protected:
		FWindowSpecification m_WindowSpecification;
	};
}
