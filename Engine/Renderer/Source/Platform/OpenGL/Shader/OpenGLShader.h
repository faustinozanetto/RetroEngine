#pragma once

#include "map"
#include "glad/glad.h"
#include "Renderer/Shader/Shader.h"

namespace Retro::Renderer
{
    enum ShaderType
    {
        Vertex = GL_VERTEX_SHADER,
        Fragment = GL_FRAGMENT_SHADER,
    };

    class OpenGLShader : public Shader
    {
    public:
        /* Constructor & Destructor */
        OpenGLShader(const std::string& vertexPath, const std::string& fragmentPath);
        ~OpenGLShader() override;

        /* Methods */
        void Bind() override;
        void UnBind() override;

        void SetFloat(const std::string& uniform, float value) override;
        void SetVecFloat2(const std::string& uniform, const glm::vec2& value) override;
        void SetVecFloat3(const std::string& uniform, const glm::vec3& value) override;
        void SetVecFloat4(const std::string& uniform, const glm::vec4& value) override;
        void SetInt(const std::string& uniform, int value) override;
        void SetVecInt2(const std::string& uniform, const glm::ivec2& value) override;
        void SetVecInt3(const std::string& uniform, const glm::ivec3& value) override;
        void SetVecInt4(const std::string& uniform, const glm::ivec4& value) override;
        void SetMat3(const std::string& uniform, const glm::mat3& value) override;
        void SetMat4(const std::string& uniform, const glm::mat4& value) override;
        
        unsigned int GetUniformLocation(const std::string& uniform) override;

    private:
        std::string ParseShaderContentsFromFile(const std::string& shaderSource);
        std::map<ShaderType, std::string> ProcessShaderSources(const std::string& vertexSource,
                                                               const std::string& fragmentSource);

        void CompileShader();

    private:
        std::map<ShaderType, std::string> m_ShaderSources;
        std::unordered_map<std::string, unsigned int> m_UniformLocations;
    };
}
