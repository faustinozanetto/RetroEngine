#pragma once

#include "core/managers/manager.h"
#include "core/assets/asset.h"
#include "renderer/materials/material.h"
#include "renderer/rendereables/model/model.h"
#include "renderer/texture/texture.h"

namespace retro
{
	class assets_manager : public manager
	{
	public:
		/* Constructor & Destructor */
		assets_manager();
		~assets_manager() override;

		/* Methods */
		void shutdown() override;

		/* Assets creation */
		shared<renderer::texture> create_texture(const renderer::texture_specification& texture_specification);
		shared<renderer::material> create_material(const renderer::material_specification& material_specification);
		shared<renderer::model> create_model(const renderer::model_specification& model_specification);
		shared<renderer::shader> create_shader(const renderer::shader_specification& shader_specification);

		std::unordered_map<shared<uuid>, shared<asset>>& get_assets_by_type(asset_type type);

		const std::unordered_map<asset_type, std::unordered_map<shared<uuid>, shared<asset>>>& get_assets() const
		{
			return m_assets;
		}

		/* Instantiate */
		static unique<assets_manager> create();

	private:
		std::unordered_map<asset_type, std::unordered_map<shared<uuid>, shared<asset>>> m_assets;
	};
}
