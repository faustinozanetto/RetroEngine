#pragma once

#include "editor_panel.h"

#include "renderer/renderer/render_passes/render_pass.h"

namespace retro::editor
{
	class render_passes_panel : public editor_panel
	{
	public:
		render_passes_panel();
		~render_passes_panel() override;

		void on_update_panel(float time) override;
		void on_render_panel() override;

	private:
		void render_pass_details(const shared<renderer::render_pass>& render_pass, const std::string& name) const;
	};
}
