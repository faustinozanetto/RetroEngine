#pragma once

#include "core/managers/manager.h"
#include "core/Assets/asset.h"
#include "Renderer/Shader/Shader.h"
#include "Renderer/Renderables/Model/Model.h"

namespace Retro
{
	class assets_mangaer : public manager
	{
	public:
		/* Constructor & Destructor */
		assets_mangaer();
		~assets_mangaer() override;

		/* Methods */
		void shutdown() override;
		void register_asset(shared<asset> asset);

		const std::unordered_map<asset_type, std::unordered_map<shared<UUID>, shared<asset>>> &get_assets() const
		{
			return m_assets;
		}

		/* Instantiate */
		static unique<assets_mangaer> create();

	private:
		std::unordered_map<asset_type, std::unordered_map<shared<UUID>, shared<asset>>> m_assets;
	};
}
