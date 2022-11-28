#pragma once

#include "glm/glm.hpp"
#include <glm/gtc/quaternion.hpp>

namespace retro::renderer
{
	struct camera_specification
	{
		float fov;
		float near_plane;
		float far_plane;

		camera_specification(float fov, float near_plane, float far_plane) : fov(fov), near_plane(near_plane), far_plane(far_plane) {}
	};
	class camera
	{
	public:
		camera(camera_specification& camera_specification);
		~camera() = default;

		void update();

		void set_viewport_size(int width, int height);
		void setup_projection_matrix();
		void update_view_matrix();

		void move(const glm::vec3& position, const glm::vec3& dir, float amount);

		const glm::mat4& get_view_matrix() const { return m_view_matrix; }
		const glm::mat4& get_projection_matrix() const { return m_projection_matrix; }
		const glm::mat4& get_view_projection() const { return m_projection_matrix * m_view_matrix; }

		glm::vec3& get_position() { return m_position; }
		void set_position(const glm::vec3& position);

		const glm::vec3& get_direction() const { return m_direction; }
		void set_direction(const glm::vec3& direction);

		const glm::quat& get_orientation() const { return m_orientation; }
		void set_orientation(const glm::quat& orientation);

		float get_fov() { return m_fov; }

		void set_fov(float fov)
		{
			m_fov = fov;
			setup_projection_matrix();
		}

		float get_near_plane() const { return m_near_plane; }

		void set_near_plane(float near_plane)
		{
			m_near_plane = near_plane;
			setup_projection_matrix();
		}

		float get_far_plane() const { return m_far_plane; }

		void set_far_plane(float far_plane)
		{
			m_far_plane = far_plane;
			setup_projection_matrix();
		}

		float get_aspect_ratio() const { return m_aspect_ratio; }
		void set_aspect_ratio(float aspect_ratio) { m_aspect_ratio = aspect_ratio; }

	private:
		glm::mat4 m_projection_matrix{};
		glm::mat4 m_view_matrix{};

		glm::quat m_orientation = glm::vec3(0.0f);
		glm::vec3 m_position = glm::vec3(0.0f);
		glm::vec3 m_direction = glm::vec3(0.0f, 0.0f, -1.0f);

		glm::vec2 m_view_size = { 1920.0f, 1080.0f };

		float m_fov = 50.0f;
		float m_aspect_ratio = 16.0f / 9.0f;
		float m_near_plane = 0.1f;
		float m_far_plane = 500.0f;

		float m_sensitivity = 0.25f;
		float m_move_speed = 8.5f;

		bool m_dirty = true;
	};
}
