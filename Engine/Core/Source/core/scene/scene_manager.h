#pragma once

#include "scene.h"
#include "core/utils/singleton.h"

namespace retro
{
	class scene_manager : public singleton<scene_manager>
	{
	public:
		static void set_active_scene(const shared<scene>& scene) { s_active_scene = scene; }
		static shared<scene>& get_active_scene() { return s_active_scene; }

	private:
		static shared<scene> s_active_scene;
	};
}
