#include "pch.h"
#include "Asset.h"
#include "Core/Application/RetroApplication.h"

namespace Retro
{
	Asset::Asset()
	{
		m_UUID = CreateShared<UUID>();
		m_AssetType = AssetType::None;
		RetroApplication::GetApplication().GetAssetsManager()->RegisterAsset(Shared<Asset>(this));
	}

	Asset::Asset(AssetType assetType)
	{
		m_UUID = CreateShared<UUID>();
		m_AssetType = assetType;
		RetroApplication::GetApplication().GetAssetsManager()->RegisterAsset(Shared<Asset>(this));
	}

	const Shared<UUID>& Asset::GetUUID() const
	{
		return m_UUID;
	}

	std::string Asset::GetAssetToString(AssetType assetType)
	{
		switch (assetType)
		{
		case AssetType::None: return "None";
		case AssetType::Model: return "Model";
		case AssetType::Texture: return "Texture";
		case AssetType::Shader: return "Shader";
		}
		return "";
	}

	static Shared<Asset> Create(AssetType assetType)
	{
		return CreateShared<Asset>(assetType);
	}
}
