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

		/**
		 * \brief Creates a texture using the given texture_specification parameters.
		 * \param texture_specification Texture specification parameter.
		 * \return A shared pointer containing the texture.
		 */
		shared<renderer::texture> create_texture(const renderer::texture_specification& texture_specification);
		/**
		 * \brief Creates a texture using width, height and raw data.
		 * \param width Width of the texture.
		 * \param height Height of the texture.
		 * \param data data Data of the texture in raw format. For example when loading models, sometimes textures are embedded.
		 * \return A shared pointer containing the texture.
		 */
		shared<renderer::texture> create_texture(uint32_t width, uint32_t height, const unsigned char* data);
		/**
		 * \brief Creates a texture using width, height, channels, and already loaded data of the texture.
		 * \param width Width of the texture.
		 * \param height Height of the texture.
		 * \param channels Channels of the texture.
		 * \param data Data of the texture that was previously loaded, for example used when loading textures with multiple threads.
		 * \return A shared pointer containing the texture.
		 */
		shared<renderer::texture> create_texture(uint32_t width, uint32_t height, uint32_t channels, const unsigned char* data);

		/**
		 * \brief Creates a vector of textures by a given vector of file paths using multi-threading for a faster load time.
		 * \param texture_paths Vector of texture file paths.
		 * \return A vector of shared pointers of textures.
		 */
		std::vector<shared<renderer::texture>> create_textures(const std::vector<std::string>& texture_paths);
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
