#pragma once

#include "Core/Base.h"
#include "Core/Assets/Asset.h"
#include "Renderer/Renderer/GraphicsObject.h"

#include "glm/glm.hpp"

namespace Retro::Renderer
{
    class Shader : public GraphicsObject, public Asset
    {
    public:
        /* Destructor */
        virtual ~Shader() = default;

        /* Methods */
        virtual void SetFloat(const std::string& uniform, float value) = 0;
        virtual void SetVecFloat2(const std::string& uniform, const glm::vec2& value) = 0;
        virtual void SetVecFloat3(const std::string& uniform, const glm::vec3& value) = 0;
        virtual void SetVecFloat4(const std::string& uniform, const glm::vec4& value) = 0;
        virtual void SetInt(const std::string& uniform, int value) = 0;
        virtual void SetVecInt2(const std::string& uniform, const glm::ivec2& value) = 0;
        virtual void SetVecInt3(const std::string& uniform, const glm::ivec3& value) = 0;
        virtual void SetVecInt4(const std::string& uniform, const glm::ivec4& value) = 0;
        virtual void SetMat3(const std::string& uniform, const glm::mat3& value) = 0;
        virtual void SetMat4(const std::string& uniform, const glm::mat4& value) = 0;

        virtual int GetUniformLocation(const std::string& uniform) = 0;

        /* Instaintiate */
        static Ref<Shader> Create(const std::string& vertexPath, const std::string& fragmentPath);
    };
}
