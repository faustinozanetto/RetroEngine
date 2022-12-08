﻿#include "pch.h"

#include "model.h"
#include "core/application/retro_application.h"
#include <assimp/postprocess.h>

namespace retro::renderer
{
	model::model(const model_specification& model_specification) : asset(asset_type::model)
	{
		logger::line();
		m_model_specification = model_specification;
		logger::info("model::model | Loading model:");
		logger::info("  - File: " + m_model_specification.file_path);
		load_model_from_path(m_model_specification.file_path);
		logger::line();
	}

	std::vector<shared<renderable>>& model::get_model_renderables()
	{
		return m_renderables;
	}

	bool model::load_model_from_path(const std::string& path)
	{
		// Create assimp importer.
		Assimp::Importer importer;
		const aiScene* scene = importer.ReadFile(
			path,
			aiProcess_Triangulate | aiProcess_GenSmoothNormals | aiProcess_JoinIdenticalVertices |
			aiProcess_OptimizeMeshes | aiProcess_ImproveCacheLocality | aiProcess_GenUVCoords | aiProcess_CalcTangentSpace);
		m_assimp_scene = scene;
		// Error handling.
		if (!scene || scene->mFlags & AI_SCENE_FLAGS_INCOMPLETE || !scene->mRootNode)
		{
			const std::string& error = importer.GetErrorString();
			logger::error("model::load_model_from_path | Assimp error: " + error);
			return false;
		}
		logger::info("	- Materials: " + m_assimp_scene->mNumMaterials);
		// Retrieve the directory path of the filepath.
		m_directory_path = path.substr(0, path.find_last_of('/'));
		// Process the root node recursively.
		return parse_model_node(m_assimp_scene->mRootNode);
	}

	bool model::parse_model_node(const aiNode* node)
	{
		for (int i = 0; i < node->mNumMeshes; i++)
		{
			// The node object only contains indices to index the actual objects in the scene.
			// The scene contains all the data, node is just to keep stuff organized (like relations between nodes).
			const aiMesh* mesh = m_assimp_scene->mMeshes[node->mMeshes[i]];
			m_renderables.push_back(parse_renderable(mesh, i));
		}

		// After we've processed all of the meshes (if any) we then recursively process each of the children nodes.
		for (int i = 0; i < node->mNumChildren; i++)
		{
			parse_model_node(node->mChildren[i]);
		}

		/*
		for (int i = 0; i < m_assimp_scene->mNumMaterials; i++)
		{
			aiMaterial* assimp_mat = m_assimp_scene->mMaterials[i];
			material_texture albedoTexture = {
				nullptr, false
			};

			material_texture normalTexture = {
				nullptr, false
			};

			material_texture metalRoughTexture = {
				nullptr, false
			};

			material_texture aoTexture = {
				nullptr, false
			};
			const std::map<material_texture_type, material_texture> textures = {
				{material_texture_type::albedo, albedoTexture},
				{material_texture_type::normal, normalTexture},
				{material_texture_type::metallic, metalRoughTexture},
				{material_texture_type::roughness, metalRoughTexture},
				{material_texture_type::ambient_occlusion, aoTexture}
			};

			const material_specification material_specification = {
				textures,
				glm::vec4(1.0f, 1.0f, 1.0f, 1.0f),
				1.0f,
				1.0f,
				1.0f,
			};

			shared<material> mat = retro_application::get_application().get_assets_manager()->create_material(material_specification);
			m_materials.insert(std::pair(i, mat));
		}
		*/

		return true;
	}

	shared<renderable> model::parse_renderable(const aiMesh* mesh, int index)
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
			normals.x = mesh->mNormals[i].x;
			normals.y = mesh->mNormals[i].y;
			normals.z = mesh->mNormals[i].z;
			// Tex coords
			glm::vec2 texCoords(0.0f);
			glm::vec3 tangent(0.0f);
			glm::vec3 bitangent(0.0f);

			// texture coordinates
			if (mesh->HasTextureCoords(0)) // does the mesh contain texture coordinates?
			{
				// tex coords
				texCoords.x = mesh->mTextureCoords[0][i].x;
				texCoords.y = mesh->mTextureCoords[0][i].y;
				// tangent
				tangent.x = mesh->mTangents[i].x;
				tangent.y = mesh->mTangents[i].y;
				tangent.z = mesh->mTangents[i].z;
				// bitangent
				bitangent.x = mesh->mBitangents[i].x;
				bitangent.y = mesh->mBitangents[i].y;
				bitangent.z = mesh->mBitangents[i].z;
			}
			else {
				texCoords = glm::vec2(0.0f, 0.0f);
			}

			// Push to the vector.
			renderable_vertex vertex = { position, texCoords, normals, tangent, bitangent };
			vertices.emplace_back(vertex);
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
		if (mesh->mMaterialIndex > 0)
		{
			// process materials
			aiMaterial* assimp_mat = m_assimp_scene->mMaterials[mesh->mMaterialIndex];

			// 1. diffuse maps
			std::vector<renderable_texture> diffuseMaps = parse_material_texture(
				assimp_mat, aiTextureType_BASE_COLOR, "texture_diffuse");
			textures.insert(textures.end(), diffuseMaps.begin(), diffuseMaps.end());
			// 2. normal maps
			std::vector<renderable_texture> normalMaps = parse_material_texture(
				assimp_mat, aiTextureType_NORMAL_CAMERA, "texture_normal");
			textures.insert(textures.end(), normalMaps.begin(), normalMaps.end());
			// 3. metal maps
			std::vector<renderable_texture> metalMaps = parse_material_texture(
				assimp_mat, aiTextureType_METALNESS, "texture_metallic");
			textures.insert(textures.end(), metalMaps.begin(), metalMaps.end());
			// 4. rough maps
			std::vector<renderable_texture> roughMaps = parse_material_texture(
				assimp_mat, aiTextureType_DIFFUSE_ROUGHNESS, "texture_roughness");
			textures.insert(textures.end(), roughMaps.begin(), roughMaps.end());
			// 5. AO maps
			std::vector<renderable_texture> aoMaps = parse_material_texture(
				assimp_mat, aiTextureType_AMBIENT_OCCLUSION, "texture_ao");
			textures.insert(textures.end(), aoMaps.begin(), aoMaps.end());
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
			logger::info("Material texture: " + *str.C_Str());
			bool skip = false;
			for (unsigned int j = 0; j < m_textures_loaded.size(); j++)
			{
				if (std::strcmp(m_textures_loaded[j].path.data(), str.C_Str()) == 0)
				{
					textures.push_back(m_textures_loaded[j]);
					skip = true;
					// a texture with the same filepath has already been loaded, continue to next one. (optimization)
					break;
				}
			}
			if (!skip)
			{
				std::string texture_path = m_directory_path + "/" + str.C_Str();
				shared<texture> texture = retro_application::get_application().get_assets_manager()->create_texture(
					{ texture_path, texture_filtering::linear, texture_wrapping::clamp_edge });
				renderable_texture rendereable_texture;
				m_textures.push_back(texture);
				rendereable_texture.id = texture->get_object_handle();
				rendereable_texture.type = type_name;
				rendereable_texture.path = texture_path;
				textures.push_back(rendereable_texture);
				m_textures_loaded.push_back(rendereable_texture); // add to loaded textures
			}
		}
		return textures;
	}

	shared<model> model::create(const model_specification& model_specification)
	{
		return create_shared<model>(model_specification);
	}
}