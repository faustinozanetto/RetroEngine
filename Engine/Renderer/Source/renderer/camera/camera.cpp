#include "pch.h"

#include "camera.h"

#define GLM_ENABLE_EXPERIMENTAL
#include <glm/gtx/quaternion.hpp>

namespace retro::renderer
{
	camera::camera(camera_specification& camera_specification)
	{
		m_fov = camera_specification.fov;
		m_near_plane = camera_specification.near_plane;
		m_far_plane = camera_specification.far_plane;

		// Setup projection matrix
		setup_projection_matrix();

		// Update view matrix.
		update_view_matrix();
	}

	void camera::update()
	{
		if (m_dirty)
		{
			update_view_matrix();
			m_dirty = false;
		}
	}

	void camera::set_viewport_size(int width, int height)
	{
		m_view_size = { width, height };
		setup_projection_matrix();
	}

	void camera::setup_projection_matrix()
	{
		m_aspect_ratio = m_view_size.x / m_view_size.y;
		// Create perspective matrix
		m_projection_matrix = glm::perspective(glm::radians(m_fov), m_aspect_ratio, m_near_plane, m_far_plane);
	}

	void camera::update_view_matrix()
	{
		// Update view matrix.
		glm::mat4 R = glm::mat4_cast(m_orientation);
		glm::mat4 T = glm::translate(glm::mat4(1.0f), -m_position);
		m_view_matrix = R * T;
	}

	void camera::move(const glm::vec3& position, const glm::vec3& dir, float amount)
	{
		set_position(position + (dir * amount));
	}

	void camera::set_position(const glm::vec3& position)
	{
		m_position = position;
		m_dirty = true;
	}

	void camera::set_direction(const glm::vec3& direction)
	{
		m_direction = direction;
		m_dirty = true;
	}

	void camera::set_orientation(const glm::quat& orientation)
	{
		m_orientation = orientation;
		m_direction = glm::normalize(glm::conjugate(m_orientation) * glm::vec3(0.0f, 0.0f, 1.0f));
		m_dirty = true;
	}
}