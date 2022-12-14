#include "pch.h"

#include "assets_manager.h"

#include <execution>

#include "core/assert.h"

namespace retro
{
	assets_manager::assets_manager() : manager("AssetsManager")
	{
	}

	assets_manager::~assets_manager()
	{
	}

	void assets_manager::shutdown()
	{
	}

	shared<renderer::texture_3d> assets_manager::create_texture_3d(
		const renderer::texture_specification& texture_specification)
	{
		auto texture = renderer::texture_3d::create(texture_specification);
		m_assets[asset_type::texture].insert(std::make_pair(texture->get_uuid(), texture));
		RETRO_CORE_ASSERT(texture, "Failed to create texture.");

		return texture;
	}

	shared<renderer::texture_2d> assets_manager::create_texture_2d(
		const renderer::texture_specification& texture_specification)
	{
		auto texture = renderer::texture_2d::create(texture_specification);
		m_assets[asset_type::texture].insert(std::make_pair(texture->get_uuid(), texture));
		RETRO_CORE_ASSERT(texture, "Failed to create texture.");

		return texture;
	}

	shared<renderer::texture_2d> assets_manager::create_texture_2d(uint32_t width, uint32_t height, const unsigned char* data)
	{
		auto texture = renderer::texture_2d::create(width, height, data);
		m_assets[asset_type::texture].insert(std::make_pair(texture->get_uuid(), texture));
		RETRO_CORE_ASSERT(texture, "Failed to create texture.");

		return texture;
	}

	shared<renderer::texture_2d> assets_manager::create_texture_2d(uint32_t width, uint32_t height, const void* pixels)
	{
		auto texture = renderer::texture_2d::create(width, height, pixels);
		m_assets[asset_type::texture].insert(std::make_pair(texture->get_uuid(), texture));
		RETRO_CORE_ASSERT(texture, "Failed to create texture.");

		return texture;
	}

	shared<renderer::texture_2d> assets_manager::create_texture_2d(uint32_t width, uint32_t height, uint32_t channels,
		const unsigned char* data)
	{
		auto texture = renderer::texture_2d::create(width, height, channels, data);
		m_assets[asset_type::texture].insert(std::make_pair(texture->get_uuid(), texture));
		RETRO_CORE_ASSERT(texture, "Failed to create texture.");

		return texture;
	}

	std::map<int, shared<renderer::texture_2d>> assets_manager::create_textures_2d(
		const std::map<int, std::string>& texture_paths)
	{
		std::map<int, renderer::texture_multi_threaded> raw_textures_data;

		std::for_each(std::execution::par, texture_paths.begin(), texture_paths.end(), [&](std::pair<int, std::string> entry)
			{
				logger::info("Loading texture multi-threaded");
		logger::info("		- Path: " + entry.second);
		const renderer::texture_multi_threaded& texture_multi_threaded = renderer::texture_2d::load_texture_raw_contents(entry.second);
		raw_textures_data.insert(std::pair(entry.first, texture_multi_threaded));
			});

		std::map<int, shared<renderer::texture_2d>> textures;
		for (const auto& [identifier, tex] : raw_textures_data)
		{
			const shared<renderer::texture_2d>& asset_tex = create_texture_2d(tex.width, tex.height, tex.channels, tex.data);
			asset_tex->get_texture_specification().path = tex.path;
			textures.insert(std::pair(identifier, asset_tex));
		}

		return textures;
	}

	shared<renderer::material> assets_manager::create_material(
		const renderer::material_specification& material_specification)
	{
		auto material = renderer::material::create(material_specification);
		m_assets[asset_type::material].insert(std::make_pair(material->get_uuid(), material));
		RETRO_CORE_ASSERT(material, "Failed to create material.");

		return material;
	}

	shared<renderer::model> assets_manager::create_model(const renderer::model_specification& model_specification)
	{
		auto model = renderer::model::create(model_specification);
		m_assets[asset_type::model].insert(std::make_pair(model->get_uuid(), model));
		RETRO_CORE_ASSERT(model, "Failed to create model.");

		return model;
	}

	shared<renderer::shader> assets_manager::create_shader(const renderer::shader_specification& shader_specification)
	{
		auto shader = renderer::shader::create(shader_specification);
		m_assets[asset_type::model].insert(std::make_pair(shader->get_uuid(), shader));
		RETRO_CORE_ASSERT(shader, "Failed to create shader.");

		return shader;
	}

	std::unordered_map<shared<uuid>, shared<asset>>& assets_manager::get_assets_by_type(asset_type type)
	{
		return m_assets[type];
	}

	unique<assets_manager> assets_manager::create()
	{
		return create_unique<assets_manager>();
	}
}