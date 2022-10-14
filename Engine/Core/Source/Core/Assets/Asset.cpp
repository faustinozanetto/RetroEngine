#include "pch.h"
#include "Asset.h"

namespace Retro {
	Asset::Asset() {
		m_UUID = CreateRef<UUID>();
		m_AssetType = AssetType::None;
	}

	Asset::Asset(AssetType assetType) {
		m_UUID = CreateRef<UUID>();
		m_AssetType = assetType;
	}

	const Ref<UUID>& Asset::GetUUID() const {
		return m_UUID;
	}

	static Ref<Asset> Create(AssetType assetType) {
		return CreateRef<Asset>(assetType);
	}
}