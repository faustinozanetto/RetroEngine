#pragma once

#include "entt/entt.hpp"

namespace retro
{
	class actor;

	class scene
	{
	public:
		scene();
		scene(const std::string& scene_name);
		~scene();

		shared<actor> create_actor();

		entt::registry& get_actor_registry() { return m_actors_registry; }
		const std::vector<shared<actor>>& get_actors() { return m_actors;}
		
		static shared<scene> create(const std::string& scene_name);

	private:
		entt::registry m_actors_registry;
		std::string m_scene_name;

		std::vector<shared<actor>> m_actors;

		friend class actor;
	};
}
