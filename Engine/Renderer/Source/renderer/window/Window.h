#pragma once

#include "core/base.h"

namespace retro::renderer
{
	struct window_specification
	{
		std::string window_title;
		int width;
		int height;
		bool v_sync;

		window_specification(const std::string& window_title = "Retro Engine", int width = 1920,
			int height = 1080, bool v_sync = false) : window_title(window_title), width(width),
			height(height), v_sync(v_sync)
		{
		}
	};

	class window
	{
	public:
		/* Destructor */
		virtual ~window() = default;

		/* Methods */
		virtual void set_vsync_enabled(bool useVSync) = 0;
		virtual void set_window_title(const std::string& title) = 0;
		virtual void setup_window_callbacks() = 0;

		/* Getters */
		const window_specification& get_window_specification();
		virtual void* get_native_window() const = 0;
		bool is_vsync_enabled() const;

		/* Instantiate */
		static unique<window> create(const window_specification& window_specification = window_specification());
	protected:
		window_specification m_window_specification;
	};
}
