#pragma once

#include "glm/glm.hpp"

namespace Retro::Renderer
{
    class Camera
    {
    public:
        Camera(float fov, float nearPlane, float farPlane);
        ~Camera() = default;

        void SetViewportSize(int width, int height);
        void SetupProjectionMatrix();
        void UpdateViewMatrix();
        const glm::vec3& CalculatePosition() const;

        const glm::mat4& GetViewMatrix() const { return m_ViewMatrix; }
        const glm::mat4& GetProjectionMatrix() const { return m_ProjectionMatrix; }
        const glm::mat4& GetViewProjection() const { return m_ProjectionMatrix * m_ViewMatrix; }
        const glm::vec3& GetPosition() const { return m_Position; }
        const glm::quat& GetOrientation() const;
        const glm::vec3& GetUpDirection() const;
        const glm::vec3& GetRightDirection() const;
        const glm::vec3& GetForwardDirection() const;

        const glm::vec3& GetFocalPoint() const { return m_FocalPoint; }

        void SetFocalPoint(const glm::vec3& focalPoint)
        {
            m_FocalPoint = focalPoint;
            UpdateViewMatrix();
        }

        const glm::vec2& GetViewSize() const { return m_ViewSize; }

        void SetViewSize(const glm::vec2& size)
        {
            m_ViewSize = size;
            SetupProjectionMatrix();
        }

        float GetDistance() const { return m_Distance; }
        void SetDistance(float distance) { m_Distance = distance; }

        float GetFOV() const { return m_FOV; }

        void SetFOV(float fov)
        {
            m_FOV = fov;
            SetupProjectionMatrix();
        }

        float GetNearPlane() const { return m_Near; }

        void SetNearPlane(float nearPlane)
        {
            m_Near = nearPlane;
            SetupProjectionMatrix();
        }

        float GetFarPlane() const { return m_Far; }

        void SetFarPlane(float farPlane)
        {
            m_Far = farPlane;
            SetupProjectionMatrix();
        }

        float GetAspectRatio() const { return m_AspectRatio; }
        void SetAspectRatio(float aspectRatio) { m_AspectRatio = aspectRatio; }

    private:
        glm::mat4 m_ProjectionMatrix;
        glm::mat4 m_ViewMatrix;

        glm::vec3 m_Position = {0.0f, 0.0f, 0.0f};
        glm::vec3 m_FocalPoint = {0.0f, 0.0f, 0.0f};
        glm::vec2 m_InitialMousePosition = {0.0f, 0.0f};

        glm::vec2 m_ViewSize = {1920.0f, 1080.0f};
        float m_FOV = 50.0f;
        float m_AspectRatio = 16.0f / 9.0f;
        float m_Near = 0.1f;
        float m_Far = 10000.0f;
        float m_Pitch = 0.0f;
        float m_Yaw = 0.0f;
        float m_Distance = 10.0f;
    };
}
