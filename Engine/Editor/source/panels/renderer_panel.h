#pragma once
#include "editor_panel.h"

namespace retro::editor
{
	class renderer_panel : public editor_panel
	{
	public:
		renderer_panel();
		~renderer_panel() override;

		void on_update_panel(float time) override;
		void on_render_panel() override;

	private:
		void render_ssao_parameters() const;
		void render_fxaa_parameters() const;
		bool m_use_vsync;
		bool m_use_ssao;
	};
}
