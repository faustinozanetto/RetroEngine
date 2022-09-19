#include "pch.h"

#include "Model.h"
#include <assimp/postprocess.h>

namespace Retro::Renderer
{
    Model::Model(const std::string &modelPath)
    {
        Logger::Line();
        Logger::Info("Model::Model | Loading model:");
        Logger::Info("  - File: " + modelPath);
        LoadModelFromPath(modelPath);
        Logger::Line();
    }

    Model::~Model()
    {
    }

    const std::vector<Ref<Renderable>> &Model::GetModelRenderables()
    {
        return m_Renderables;
    }

    bool Model::LoadModelFromPath(const std::string &path)
    {
        // Assign path.
        m_ModelPath = path;
        // Create assimp importer.
        Assimp::Importer importer;
        const aiScene *scene = importer.ReadFile(
            m_ModelPath,
            aiProcess_Triangulate | aiProcess_GenSmoothNormals | aiProcess_CalcTangentSpace | aiProcess_FlipUVs |
                aiProcess_GenBoundingBoxes);
        m_AssimpScene = scene;
        // Error handling.
        if (!scene || scene->mFlags & AI_SCENE_FLAGS_INCOMPLETE || !scene->mRootNode)
        {
            const std::string error = importer.GetErrorString();
            Logger::Error("Model::LoadModelFromPath | Assimp error: " + error);
            return false;
        }
        // Process the root node recursively.
        return ProcessModelNode(m_AssimpScene->mRootNode);
    }

    bool Model::ProcessModelNode(const aiNode *node)
    {
        for (unsigned int i = 0; i < node->mNumMeshes; i++)
        {
            // The node object only contains indices to index the actual objects in the scene.
            // The scene contains all the data, node is just to keep stuff organized (like relations between nodes).
            const aiMesh *mesh = m_AssimpScene->mMeshes[node->mMeshes[i]];
            m_Renderables.push_back(ParseRenderable(mesh));
        }

        // After we've processed all of the meshes (if any) we then recursively process each of the children nodes.
        for (unsigned int i = 0; i < node->mNumChildren; i++)
        {
            ProcessModelNode(node->mChildren[i]);
        }
        return true;
    }

    Ref<Renderable> Model::ParseRenderable(const aiMesh *mesh)
    {
        // Create temp vectors.
        std::vector<RenderableVertex> vertices;
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
            vertices.push_back({position, texCoords, normals, tangent, bitangent});
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
        Logger::Info(
            "Mesh has " + std::to_string(mesh->mNumVertices) + " vertices and " + std::to_string(mesh->mNumFaces) +
            " faces.");

        return CreateRef<Renderable>(vertices, indices);
    }

    Ref<Model> Model::Create(const std::string &modelPath)
    {
        return CreateRef<Model>(modelPath);
    }
}
