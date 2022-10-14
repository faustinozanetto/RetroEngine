#include "pch.h"
#include "AssetsManager.h"

namespace Retro {
	AssetsManager::AssetsManager() : Manager("AssetssManager") {

	}

	AssetsManager::~AssetsManager() {

	}

	void AssetsManager::Shutdown() {
		m_ShaderAssets.clear();
		m_ModelAssets.clear();
	}

	void AssetsManager::RegisterShader(const Ref<Renderer::Shader>& shader) {
		m_ShaderAssets.insert(std::make_pair(shader->GetUUID(), shader));
	}

	void AssetsManager::RegisterModel(const Ref<Renderer::Model>& model) {
		m_ModelAssets.insert(std::make_pair(model->GetUUID(), model));
	}

	Scope<AssetsManager> AssetsManager::Create()
	{
		return CreateScope<AssetsManager>();
	}
}