﻿#pragma once

#include "Core/Base.h"
#include "Core/Assets/Asset.h"
#include "Renderer/Renderables/Renderable.h"
#include <assimp/Importer.hpp>
#include <assimp/scene.h>

#include "Renderer/Textures/Texture.h"

namespace Retro::Renderer
{
	class Model : public Asset
	{
	public:
		/* Constructor & Destructor */
		Model(const std::string& modelPath);
		~Model() override;

		/* Instantiate */
		static Shared<Model> Create(const std::string& modelPath);

		/* Methods */
		const std::vector<Shared<Renderable>>& GetModelRenderables();
		const std::vector<RendereableTexture>& GetEmbeddedTextures() const { return m_TexturesLoaded; }

		std::vector<RendereableTexture>
		ParseMaterialTextures(aiMaterial* mat, aiTextureType type, std::string typeName);
	private:
		bool LoadModelFromPath(const std::string& path);
		bool ProcessModelNode(const aiNode* node);
		Shared<Renderable> ParseRenderable(const aiMesh* mesh);

	private:
		const aiScene* m_AssimpScene{};
		std::vector<Shared<Texture>> m_Textures;
		std::vector<RendereableTexture> m_TexturesLoaded;
		std::vector<Shared<Renderable>> m_Renderables;
		std::string m_ModelPath;
		std::string m_DirectoryPath;
	};
}
