#pragma once

#include "Core/Utils/UUID.h"
#include "Core/Base.h"

namespace Retro
{
	enum class AssetType
	{
		None = 0,
		Model = 1,
		Texture = 2,
		Shader = 3,
	};

	class Asset
	{
	public:
		Asset();
		Asset(AssetType assetType);
		virtual ~Asset() = default;

		const Ref<UUID>& GetUUID() const;
		const AssetType GetAssetType() { return m_AssetType; }

		static std::string GetAssetToString(AssetType assetType);
		static Ref<Asset> Create(AssetType assetType);

	private:
		Ref<UUID> m_UUID;
		AssetType m_AssetType;
	};
}
