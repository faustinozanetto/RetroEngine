#include "pch.h"

#include "camera.h"

#define GLM_ENABLE_EXPERIMENTAL
#include <glm/gtx/quaternion.hpp>

namespace retro::renderer
{
    camera::camera(float fov, float near_plane, float far_plane)
    {
        m_fov = fov;
        m_near_plane = near_plane;
        m_far_plane = far_plane;

        // Setup projection matrix
        setup_projection_matrix();

        // Update view matrix.
        update_view_matrix();
    }

    void camera::set_viewport_size(int width, int height)
    {
        m_view_size = {width, height};
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
        m_position = calculate_position();
        m_view_matrix = translate(glm::mat4(1.0f), m_position) * toMat4(get_orientation());
        m_view_matrix = inverse(m_view_matrix);
    }

    void camera::rotateHorizontally(const glm::vec2& delta)
    {
        float yawSign = get_up_direction().y<0 ? -1.0f : 1.0f;
        m_yaw += yawSign * delta.x * 0.8f;
        m_pitch += delta.y * 0.8f;
    }

    const glm::vec3& camera::calculate_position() const
    {
        return m_focal_point - get_forward_direction() * m_distance;
    }

    const glm::quat& camera::get_orientation() const
    {
        return glm::quat(glm::vec3(-m_pitch, -m_yaw, 0.0f));
    }

    const glm::vec3& camera::get_up_direction() const
    {
        return rotate(get_orientation(), glm::vec3(0.0f, 1.0f, 0.0f));
    }

    const glm::vec3& camera::get_right_direction() const
    {
        return rotate(get_orientation(), glm::vec3(1.0f, 0.0f, 0.0f));
    }

    const glm::vec3& camera::get_forward_direction() const
    {
        return rotate(get_orientation(), glm::vec3(0.0f, 0.0f, -1.0f));
    }
}
