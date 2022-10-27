﻿#include "pch.h"

#include "model.h"
#include <assimp/postprocess.h>

namespace retro::renderer
{
	model::model(const std::string& model_path) : asset(asset_type::model)
	{
		logger::line();
		logger::info("model::model | Loading model:");
		logger::info("  - File: " + model_path);
		load_model_from_path(model_path);
		logger::line();
	}

	model::~model()
	{
	}

	const std::vector<shared<renderable>>& model::get_model_renderables()
	{
		return m_renderables;
	}

	bool model::load_model_from_path(const std::string& path)
	{
		// Assign path.
		m_model_path = path;
		// Create assimp importer.
		Assimp::Importer importer;
		const aiScene* scene = importer.ReadFile(
			m_model_path,
			aiProcess_Triangulate | aiProcess_GenSmoothNormals | aiProcess_CalcTangentSpace | aiProcess_FlipUVs |
			aiProcess_GenBoundingBoxes);
		m_assimp_scene = scene;
		// Error handling.
		if (!scene || scene->mFlags & AI_SCENE_FLAGS_INCOMPLETE || !scene->mRootNode)
		{
			const std::string& error = importer.GetErrorString();
			logger::error("model::load_model_from_path | Assimp error: " + error);
			return false;
		}
		// Retrieve the directory path of the filepath.
		m_directory_path = m_model_path.substr(0, m_model_path.find_last_of('\\'));
		// Process the root node recursively.
		return parse_model_node(m_assimp_scene->mRootNode);
	}

	bool model::parse_model_node(const aiNode* node)
	{
		for (unsigned int i = 0; i < node->mNumMeshes; i++)
		{
			// The node object only contains indices to index the actual objects in the scene.
			// The scene contains all the data, node is just to keep stuff organized (like relations between nodes).
			const aiMesh* mesh = m_assimp_scene->mMeshes[node->mMeshes[i]];
			m_renderables.push_back(parse_renderable(mesh));
		}

		// After we've processed all of the meshes (if any) we then recursively process each of the children nodes.
		for (unsigned int i = 0; i < node->mNumChildren; i++)
		{
			parse_model_node(node->mChildren[i]);
		}
		return true;
	}

	shared<renderable> model::parse_renderable(const aiMesh* mesh)
	{
		// Create temp vectors.
		std::vector<renderable_vertex> vertices;
		std::vector<unsigned int> indices;
		std::vector<renderable_texture> textures;

		// Process vertices.
		for (unsigned int i = 0; i < mesh->mNumVertices; i++)
		{
			// Position.
			glm::vec3 position;
			position.x = mesh->mVertices[i].x;
			position.y = mesh->mVertices[i].y;
			position.z = mesh->mVertices[i].z;
			// Normals
			glm::vec3 normals(0.0f);
			if (mesh->HasNormals())
			{
				normals.x = mesh->mNormals[i].x;
				normals.y = mesh->mNormals[i].y;
				normals.z = mesh->mNormals[i].z;
			}
			// Tex coords
			glm::vec2 texCoords(0.0f);
			glm::vec3 tangent(0.0f);
			glm::vec3 bitangent(0.0f);
			if (mesh->HasTextureCoords(0))
			{
				texCoords.x = mesh->mTextureCoords[0][i].x;
				texCoords.y = 1 - mesh->mTextureCoords[0][i].y;

				tangent.x = mesh->mTangents[i].x;
				tangent.y = mesh->mTangents[i].y;
				tangent.z = mesh->mTangents[i].z;

				bitangent.x = mesh->mBitangents[i].x;
				bitangent.y = mesh->mBitangents[i].y;
				bitangent.z = mesh->mBitangents[i].z;
			}
			// Push to the vector.
			vertices.emplace_back(position, texCoords, normals, tangent, bitangent);
		}
		// Process indices.
		for (unsigned int i = 0; i < mesh->mNumFaces; i++)
		{
			aiFace face = mesh->mFaces[i];
			// Retrieve all indices of the face and store them in the indices vector.
			for (unsigned int j = 0; j < face.mNumIndices; j++)
			{
				indices.push_back(face.mIndices[j]);
			}
		}
		logger::info(
			"Mesh has " + std::to_string(mesh->mNumVertices) + " vertices and " + std::to_string(mesh->mNumFaces) +
			" faces.");

		/*
		if (mesh->mMaterialIndex >= 0)
		{
			// process materials
			aiMaterial* material = m_AssimpScene->mMaterials[mesh->mMaterialIndex];

			// 1. diffuse maps
			std::vector<RendereableTexture> diffuseMaps = ParseMaterialTextures(
				material, aiTextureType_DIFFUSE, "texture_diffuse");
			textures.insert(textures.end(), diffuseMaps.begin(), diffuseMaps.end());
			// 2. specular maps
			std::vector<RendereableTexture> specularMaps = ParseMaterialTextures(
				material, aiTextureType_SPECULAR, "texture_specular");
			textures.insert(textures.end(), specularMaps.begin(), specularMaps.end());
			// 3. normal maps
			std::vector<RendereableTexture> normalMaps = ParseMaterialTextures(
				material, aiTextureType_DISPLACEMENT, "texture_normal");
			std::vector<RendereableTexture> normalMaps2 = ParseMaterialTextures(
				material, aiTextureType_HEIGHT, "texture_normal");
			textures.insert(textures.end(), normalMaps.begin(), normalMaps.end());
			textures.insert(textures.end(), normalMaps2.begin(), normalMaps2.end());
			// 4. height maps
			std::vector<RendereableTexture> heightMaps = ParseMaterialTextures(
				material, aiTextureType_AMBIENT, "texture_height");
			textures.insert(textures.end(), heightMaps.begin(), heightMaps.end());
		}
		*/

		if (textures.empty())
		{
			logger::info("The model has no textures.");
		}

		return create_shared<renderable>(vertices, indices, textures);
	}

	std::vector<renderable_texture> model::parse_material_texture(aiMaterial* mat, aiTextureType type,
	                                                             std::string type_name)
	{
		std::vector<renderable_texture> textures;
		for (unsigned int i = 0; i < mat->GetTextureCount(type); i++)
		{
			aiString str;
			mat->GetTexture(type, i, &str);
			std::string file = str.C_Str();
			logger::info("Material texture: " + file);
			bool skip = false;
			for (auto& j : m_textures_loaded)
			{
				if (std::strcmp(j.path.data(), str.C_Str()) == 0)
				{
					textures.push_back(j);
					skip = true;
					break;
				}
			}
			if (!skip)
			{
				// if texture hasn't been loaded already, load it
				file = m_directory_path + '\\' + file;

				shared<texture> texture;
				if (const auto& tex = m_assimp_scene->GetEmbeddedTexture(str.C_Str()))
				{
					logger::info("Found embedded texture" + *tex->mFilename.C_Str());
					const uint32_t width = tex->mWidth;
					const uint32_t height = tex->mHeight;
					texture = texture::create(
						width, height, reinterpret_cast<unsigned char*>(tex->pcData));
				}
				else
				{
					texture = texture::create({file, texture_filtering::nearest, texture_wrapping::repeat});
				}
				if (texture)
				{
					renderable_texture rendereable_texture;
					m_textures.push_back(texture);
					rendereable_texture.id = texture->get_object_handle();
					rendereable_texture.type = type_name;
					rendereable_texture.path = str.C_Str();
					textures.push_back(rendereable_texture);
					m_textures_loaded.push_back(rendereable_texture); // add to loaded textures
				}
			}
		}
		return textures;
	}

	shared<model> model::create(const std::string& model_path)
	{
		return create_shared<model>(model_path);
	}
}