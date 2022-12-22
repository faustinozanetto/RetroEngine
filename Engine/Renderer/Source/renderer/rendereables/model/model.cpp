#include "pch.h"

#include "model.h"
#include "core/application/retro_application.h"
#include <assimp/postprocess.h>

#include "core/assets/assets_manager.h"

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

	model::~model()
	{
	}

	std::vector<shared<renderable>>& model::get_model_renderables()
	{
		return m_renderables;
	}

	void model::load_model_from_path(const std::string& path)
	{
		// Create assimp importer.
		Assimp::Importer importer;
		const aiScene* scene = importer.ReadFile(
			path,
			aiProcess_Triangulate | aiProcess_GenSmoothNormals | aiProcess_JoinIdenticalVertices |
			aiProcess_OptimizeMeshes | aiProcess_CalcTangentSpace | aiProcess_PreTransformVertices);
		m_assimp_scene = scene;
		// Error handling.
		if (!scene || scene->mFlags & AI_SCENE_FLAGS_INCOMPLETE || !scene->mRootNode)
		{
			const std::string& error = importer.GetErrorString();
			logger::error("model::load_model_from_path | Assimp error: " + error);
		}
		logger::info("	- Materials: " + m_assimp_scene->mNumMaterials);
		// Retrieve the directory path of the filepath.
		m_directory_path = path.substr(0, path.find_last_of('/'));
		// Process the root node recursively.
		parse_model_node(m_assimp_scene->mRootNode);
		parse_model_materials();
	}

	void model::parse_model_node(const aiNode* node)
	{
		for (int i = 0; i < node->mNumMeshes; i++)
		{
			// The node object only contains indices to index the actual objects in the scene.
			// The scene contains all the data, node is just to keep stuff organized (like relations between nodes).
			m_renderables.push_back(parse_renderable(m_assimp_scene->mMeshes[node->mMeshes[i]], i));
		}

		// After we've processed all of the meshes (if any) we then recursively process each of the children nodes.
		for (int i = 0; i < node->mNumChildren; i++)
		{
			parse_model_node(node->mChildren[i]);
		}
	}

	shared<renderable> model::parse_renderable(const aiMesh* mesh, int index)
	{
		// Create temp vectors.
		std::vector<renderable_vertex> vertices;
		std::vector<renderable_texture> textures;
		std::vector<unsigned int> indices;

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
				texCoords = glm::vec2(mesh->mTextureCoords[0][i].x, mesh->mTextureCoords[0][i].y);
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

		if (m_assimp_scene->HasMaterials())
		{
			aiMaterial* assimp_mat = m_assimp_scene->mMaterials[mesh->mMaterialIndex];

			std::vector<renderable_texture> albedo_maps = parse_mat_texture(
				assimp_mat, aiTextureType_DIFFUSE, "texture_diffuse");

			std::vector<renderable_texture> normal_maps = parse_mat_texture(
				assimp_mat, aiTextureType_NORMALS, "texture_normal");

			std::vector<renderable_texture> roughness_maps = parse_mat_texture(
				assimp_mat, aiTextureType_DIFFUSE_ROUGHNESS, "texture_roughness");

			std::vector<renderable_texture> ao_maps = parse_mat_texture(
				assimp_mat, aiTextureType_AMBIENT_OCCLUSION, "texture_ao");

			std::map<material_texture_type, std::string> textures{};
			if (!albedo_maps.empty()) {
				textures.insert(std::pair(material_texture_type::albedo, albedo_maps[0].path));
			}
			if (!normal_maps.empty()) {
				textures.insert(std::pair(material_texture_type::normal, normal_maps[0].path));
			}
			if (!roughness_maps.empty()) {
				textures.insert(std::pair(material_texture_type::roughness, roughness_maps[0].path));
			}
			if (!ao_maps.empty()) {
				textures.insert(std::pair(material_texture_type::ambient_occlusion, ao_maps[0].path));
			}
			std::pair<int, std::map<material_texture_type, std::string>> texts = std::pair(mesh->mMaterialIndex, textures);
			m_material_textures.insert(texts);
		}

		shared<renderable> model_renderable = create_shared<renderable>(vertices, indices, textures);
		model_renderable->set_name(mesh->mName.C_Str());
		return model_renderable;
	}

	std::vector<renderable_texture> model::parse_mat_texture(aiMaterial* mat, aiTextureType type, const std::string& type_name)
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
				std::string filename = str.C_Str();
				filename = m_directory_path + '\\' + filename;

				/*
				if (const auto tex = m_assimp_scene->GetEmbeddedTexture(str.C_Str())) {
					const auto width = tex->mWidth;
					const auto height = tex->mHeight;
					texture = retro_application::get_application().get_assets_manager()->create_texture(
						width, height, reinterpret_cast<unsigned char*>(tex->pcData));
				}
				else {
					texture = retro_application::get_application().get_assets_manager()->create_texture(
						{ filename, texture_filtering::linear ,texture_wrapping::clamp_edge });
				}
				*/

				renderable_texture rendereable_texture;
				rendereable_texture.type = type_name;
				rendereable_texture.path = filename;
				textures.push_back(rendereable_texture);
				m_textures_loaded.push_back(rendereable_texture); // add to loaded textures
			}
		}
		return textures;
	}

	void model::parse_model_materials()
	{
		if (m_material_textures.empty()) return;

		// Iterate through all the loaded material textures.
		for (auto& material_textures : m_material_textures)
		{
			// Create map that maps int (texture_typ) to string (tex_path) with the mat_tex.
			const auto& [mat_idx, textures] = material_textures;
			std::map<int, std::string> input_textures{};
			for (const auto& [tex_type, path] : textures)
			{
				input_textures.insert(std::pair(static_cast<int>(tex_type), path));
			}

			// Use the input map for loading the textures using multi threading.
			const std::map<int, shared<texture_2d>>& loaded_textures = assets_manager::get().create_textures_2d(input_textures);

			material_texture albedo_mat_tex = {
				nullptr, false
			};
			if (loaded_textures.contains(static_cast<int>(material_texture_type::albedo))) {
				albedo_mat_tex.mat_texture = loaded_textures.at(static_cast<int>(material_texture_type::albedo));
				albedo_mat_tex.enabled = true;
			}

			material_texture normal_mat_tex = {
				nullptr, false
			};
			if (loaded_textures.contains(static_cast<int>(material_texture_type::normal))) {
				normal_mat_tex.mat_texture = loaded_textures.at(static_cast<int>(material_texture_type::normal));
				normal_mat_tex.enabled = true;
			}

			material_texture roughness_mat_tex = {
				nullptr, false
			};
			if (loaded_textures.contains(static_cast<int>(material_texture_type::roughness))) {
				roughness_mat_tex.mat_texture = loaded_textures.at(static_cast<int>(material_texture_type::roughness));
				roughness_mat_tex.enabled = true;
			}

			material_texture ao_mat_tex = {
				nullptr, false
			};
			if (loaded_textures.contains(static_cast<int>(material_texture_type::ambient_occlusion))) {
				ao_mat_tex.mat_texture = loaded_textures.at(static_cast<int>(material_texture_type::ambient_occlusion));
				ao_mat_tex.enabled = true;
			}

			const std::map<material_texture_type, material_texture> mat_textures = {
				{material_texture_type::albedo, albedo_mat_tex},
				{material_texture_type::normal, normal_mat_tex},
				{material_texture_type::metallic, roughness_mat_tex},
				{material_texture_type::roughness, roughness_mat_tex},
				{material_texture_type::ambient_occlusion, ao_mat_tex}
			};

			const std::string& mat_name = m_assimp_scene->mMaterials[mat_idx]->GetName().C_Str();

			const material_specification material_specification = {
				mat_textures,
				mat_name,
				glm::vec4(1.0f, 1.0f, 1.0f, 1.0f),
				1.0f,
				1.0f,
				1.0f,
			};

			const shared<material>& created_mat = assets_manager::get().create_material(material_specification);

			m_embedded_materials.insert(std::pair(mat_idx, created_mat));
		}
	}

	shared<model> model::create(const model_specification& model_specification)
	{
		return create_shared<model>(model_specification);
	}
}