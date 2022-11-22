#pragma once

#include <entt/entt.hpp>
#include "core/assert.h"
#include "scene.h"

namespace retro
{
    class actor
    {
    public:
        static scene* s_scene;

        actor();
        actor(entt::entity handle);
        actor(entt::entity handle, scene* scene);
        ~actor();

        entt::entity get_actor_handle() const { return m_actor_handle; };

        template <typename T, typename ... Args>
        T& add_component(Args&& ...args)
        {
            RETRO_CORE_ASSERT(!has_component<T>(), "The actor already has that component.");
            return s_scene->get_actor_registry().emplace<T>(m_actor_handle, std::forward<Args>(args)...);
        }

        template <typename T>
        T& get_component()
        {
            RETRO_CORE_ASSERT(has_component<T>(), "The actor does not have that component.");
            return s_scene->get_actor_registry().get<T>(m_actor_handle);
        }

        template <typename T>
        bool has_component() const
        {
            return s_scene->get_actor_registry().has<T>(m_actor_handle);
        }

        template <typename T>
        void remove_component()
        {
            RETRO_CORE_ASSERT(has_component<T>(), "The actor does not have that component.");
            s_scene->get_actor_registry().remove<T>(m_actor_handle);
        }

        operator bool() const
        {
            return m_actor_handle != entt::null;
        }

        static shared<actor> create(entt::entity handle);

    private:
        entt::entity m_actor_handle{entt::null};
    };
}
