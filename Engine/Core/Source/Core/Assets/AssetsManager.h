#pragma once

#include "Core/Managers/Manager.h"
#include "Renderer/Shader/Shader.h"
#include "Renderer/Renderables/Model/Model.h"

namespace Retro
{
	class AssetsManager : public Manager
	{
	public:
		/* Constructor & Destructor */
		AssetsManager();
		~AssetsManager() override;

		/* Methods */
		void Shutdown() override;
		void RegisterAsset(Shared<Asset> asset);

		const std::unordered_map<AssetType, std::unordered_map<Shared<UUID>, Shared<Asset>>>& GetAssets() const
		{
			return m_Assets;
		}

		/* Instantiate */
		static Unique<AssetsManager> Create();
	private:
		std::unordered_map<AssetType, std::unordered_map<Shared<UUID>, Shared<Asset>>> m_Assets;
	};
}
