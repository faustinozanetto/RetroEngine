#pragma once

#include "core/managers/manager.h"
#include "core/assets/asset.h"
#include "renderer/materials/material.h"
#include "renderer/rendereables/model/model.h"
#include "renderer/texture/texture_3d.h"

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
		shared<renderer::texture_3d> create_texture_3d(const renderer::texture_specification& texture_specification);

		/**
		 * \brief Creates a texture using the given texture_specification parameters.
		 * \param texture_specification Texture specification parameter.
		 * \return A shared pointer containing the texture.
		 */
		shared<renderer::texture_2d> create_texture_2d(const renderer::texture_specification& texture_specification);
		/**
		 * \brief Creates a texture using width, height and raw data.
		 * \param width Width of the texture.
		 * \param height Height of the texture.
		 * \param data data Data of the texture in raw format. For example when loading models, sometimes textures are embedded.
		 * \return A shared pointer containing the texture.
		 */
		shared<renderer::texture_2d> create_texture_2d(uint32_t width, uint32_t height, const unsigned char* data);
		/**
		 * \brief Creates a texture using width, height and raw data.
		 * \param width Width of the texture.
		 * \param height Height of the texture.
		 * \param pixels Pixels of the texture in raw format.
		 * \return A shared pointer containing the texture.
		 */
		shared<renderer::texture_2d> create_texture_2d(uint32_t width, uint32_t height, const void* pixels);
		/**
		 * \brief Creates a texture using width, height, channels, and already loaded data of the texture.
		 * \param width Width of the texture.
		 * \param height Height of the texture.
		 * \param channels Channels of the texture.
		 * \param data Data of the texture that was previously loaded, for example used when loading textures with multiple threads.
		 * \return A shared pointer containing the texture.
		 */
		shared<renderer::texture_2d> create_texture_2d(uint32_t width, uint32_t height, uint32_t channels, const unsigned char* data);

		/**
		 * \brief Creates a map of textures by a given map of file paths and identifiers using multi-threading for a faster load time.
		 * \param texture_paths Map of texture file paths and identifiers.
		 * \return A map of shared pointers of textures and identifiers.
		 */
		std::map<int, shared<renderer::texture_2d>> create_textures_2d(const std::map<int, std::string>& texture_paths);
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
