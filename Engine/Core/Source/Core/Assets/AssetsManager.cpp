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

	void AssetsManager::RegisterAsset(const Shared<Asset> asset)
	{
		m_Assets[asset->GetAssetType()].insert(std::make_pair(asset->GetUUID(), asset));
	}

	Unique<AssetsManager> AssetsManager::Create()
	{
		return CreateUnique<AssetsManager>();
	}
}
