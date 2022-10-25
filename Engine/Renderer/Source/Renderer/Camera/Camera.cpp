#include "pch.h"
#include "Camera.h"

#define GLM_ENABLE_EXPERIMENTAL
#include <glm/gtx/quaternion.hpp>

namespace Retro::Renderer
{
	Camera::Camera(float fov, float nearPlane, float farPlane)
	{
		m_FOV = fov;
		m_Near = nearPlane;
		m_Far = farPlane;

		// Setup projection matrix
		SetupProjectionMatrix();

		// Update view matrix.
		UpdateViewMatrix();
	}

	void Camera::SetViewportSize(int width, int height)
	{
		m_ViewSize = {width, height};
		SetupProjectionMatrix();
	}

	void Camera::SetupProjectionMatrix()
	{
		m_AspectRatio = m_ViewSize.x / m_ViewSize.y;
		// Create perspective matrix
		m_ProjectionMatrix = glm::perspective(glm::radians(m_FOV), m_AspectRatio, m_Near, m_Far);
	}

	void Camera::UpdateViewMatrix()
	{
		// Update view matrix.
		m_Position = CalculatePosition();
		m_ViewMatrix = translate(glm::mat4(1.0f), m_Position) * toMat4(GetOrientation());
		m_ViewMatrix = inverse(m_ViewMatrix);
	}

	const glm::vec3& Camera::CalculatePosition() const
	{
		return m_FocalPoint - GetForwardDirection() * m_Distance;
	}

	const glm::quat& Camera::GetOrientation() const
	{
		return glm::quat(glm::vec3(-m_Pitch, -m_Yaw, 0.0f));
	}

	const glm::vec3& Camera::GetUpDirection() const
	{
		return rotate(GetOrientation(), glm::vec3(0.0f, 1.0f, 0.0f));
	}

	const glm::vec3& Camera::GetRightDirection() const
	{
		return rotate(GetOrientation(), glm::vec3(1.0f, 0.0f, 0.0f));
	}

	const glm::vec3& Camera::GetForwardDirection() const
	{
		return rotate(GetOrientation(), glm::vec3(0.0f, 0.0f, -1.0f));
	}
}
