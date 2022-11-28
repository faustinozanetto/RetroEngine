#pragma once

#include "core/base.h"
#include "core/assets/asset.h"
#include "renderer/rendereables/renderable.h"
#include "renderer/texture/texture.h"

#include <assimp/Importer.hpp>
#include <assimp/scene.h>

namespace retro::renderer
{
	struct model_specification
	{
		std::string file_path;

		model_specification() = default;

		model_specification(const std::string& file_path) : file_path(file_path)
		{
		}
	};

	class model final : public asset
	{
	public:
		/* Constructor & Destructor */
		model(const model_specification& model_specification);
		~model() override = default;

		/* Methods */
		std::vector<shared<renderable>>& get_model_renderables();
		const std::vector<renderable_texture>& get_embedded_textures() const { return m_textures_loaded; }

		/* Asset */
		void serialize() override;

		/* Instantiate */
		static shared<model> create(const model_specification& model_specification);

	private:
		bool load_model_from_path(const std::string& path);
		bool parse_model_node(const aiNode* node);
		shared<renderable> parse_renderable(const aiMesh* mesh);
		std::vector<renderable_texture>
			parse_material_texture(aiMaterial* mat, aiTextureType type, std::string type_name);

	private:
		const aiScene* m_assimp_scene{};
		model_specification m_model_specification;
		std::vector<shared<texture>> m_textures;
		std::vector<renderable_texture> m_textures_loaded;
		std::vector<shared<renderable>> m_renderables;
		std::string m_directory_path;
	};
}
