#include "pch.h"
#include "AssetsManager.h"

namespace Retro
{
	AssetsManager::AssetsManager() : Manager("AssetssManager")
	{
	}

	AssetsManager::~AssetsManager()
	{
	}

	void AssetsManager::Shutdown()
	{
		m_Assets.clear();
	}

	void AssetsManager::RegisterAsset(const Ref<Asset> asset)
	{
		m_Assets[asset->GetAssetType()].insert(std::make_pair(asset->GetUUID(), asset));
	}

	Scope<AssetsManager> AssetsManager::Create()
	{
		return CreateScope<AssetsManager>();
	}
}
