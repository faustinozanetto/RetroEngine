#pragma once

#include "glm/glm.hpp"

namespace retro::renderer
{
	class camera
	{
	public:
		camera(float fov, float near_plane, float far_plane);
		~camera() = default;

		void set_viewport_size(int width, int height);
		void setup_projection_matrix();
		void update_view_matrix();
		const glm::vec3& calculate_position() const;

		const glm::mat4& get_view_matrix() const { return m_view_matrix; }
		const glm::mat4& get_projection_matrix() const { return m_projection_matrix; }
		const glm::mat4& get_view_projection() const { return m_projection_matrix * m_view_matrix; }
		const glm::vec3& get_position() const { return m_position; }
		const glm::quat& get_orientation() const;
		const glm::vec3& get_up_direction() const;
		const glm::vec3& get_right_direction() const;
		const glm::vec3& get_forward_direction() const;

		const glm::vec3& get_focal_point() const { return m_focal_point; }

		void set_focal_point(const glm::vec3& focal_point)
		{
			m_focal_point = focal_point;
			update_view_matrix();
		}

		const glm::vec2& get_view_size() const { return m_view_size; }

		void set_view_size(const glm::vec2& size)
		{
			m_view_size = size;
			setup_projection_matrix();
		}

		float get_distance() const { return m_distance; }
		void set_distance(float distance) { m_distance = distance; }

		float get_fov() const { return m_fov; }

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

		float get_fra_plane() const { return m_far_plane; }

		void set_far_plane(float far_plane)
		{
			m_far_plane = far_plane;
			setup_projection_matrix();
		}

		float get_aspect_ratio() const { return m_aspect_ratio; }
		void set_aspect_ratio(float aspect_ratio) { m_aspect_ratio = aspect_ratio; }

	private:
		glm::mat4 m_projection_matrix;
		glm::mat4 m_view_matrix;

		glm::vec3 m_position = {0.0f, 0.0f, 0.0f};
		glm::vec3 m_focal_point = {0.0f, 0.0f, 0.0f};
		glm::vec2 m_initial_mouse_position = {0.0f, 0.0f};

		glm::vec2 m_view_size = {1920.0f, 1080.0f};
		float m_fov = 50.0f;
		float m_aspect_ratio = 16.0f / 9.0f;
		float m_near_plane = 0.1f;
		float m_far_plane = 10000.0f;
		float m_pitch = 0.0f;
		float m_yaw = 0.0f;
		float m_distance = 10.0f;
	};
}
