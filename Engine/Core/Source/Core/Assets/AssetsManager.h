#pragma once

#include "Core/Managers/Manager.h"
#include "Renderer/Shader/Shader.h"
#include "Renderer/Renderables/Model/Model.h"

namespace Retro {
	class AssetsManager : public Manager {
    public:
        /* Constructor & Destructor */
        AssetsManager();
        ~AssetsManager() override;

        /* Methods */
        void Shutdown() override;
        void RegisterShader(const Ref<Renderer::Shader>& shader);
        void RegisterModel(const Ref<Renderer::Model>& model);

        const std::unordered_map<Ref<UUID>, Ref<Renderer::Shader>>& GetShaderAssets() const { return m_ShaderAssets; }
        const std::unordered_map<Ref<UUID>, Ref<Renderer::Model>>& GetModelAssets() const { return m_ModelAssets; }

        /* Instantiate */
        static Scope<AssetsManager> Create();
    private:
        std::unordered_map<Ref<UUID>, Ref<Renderer::Shader>> m_ShaderAssets;
        std::unordered_map<Ref<UUID>, Ref<Renderer::Model>> m_ModelAssets;
	};
}