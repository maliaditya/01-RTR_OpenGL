#pragma once
#include "precomp.h"

namespace OGL {
    
    Logger mdlog("Model.log");

    // Recursive function to process each node
    void processNode(aiNode* node, const aiScene* scene, std::vector<Geometry>& models) {
        // Process all meshes in this node
        for (unsigned int i = 0; i < node->mNumMeshes; i++) {
            aiMesh* mesh = scene->mMeshes[node->mMeshes[i]];
            std::string meshName = mesh->mName.C_Str(); // Get mesh name

            // Empty Model to hold the mesh data
            Geometry model;
            model.name = meshName;

            // Add Vertices
            for (int j = 0; j < mesh->mNumVertices; j++) {
                aiVector3D vertex = mesh->mVertices[j];
                model.positions.emplace_back(vertex.x, vertex.y, vertex.z);  // Positions

                // Load normals
                if (mesh->HasNormals()) {
                    aiVector3D normal = mesh->mNormals[j];
                    model.normals.emplace_back(normal.x, normal.y, normal.z); // Normals
                }

                // Load UVs
                if (mesh->HasTextureCoords(0)) {
                    aiVector3D uv = mesh->mTextureCoords[0][j]; // get UV
                    model.uv.emplace_back(uv.x, uv.y); // push UV
                } else {
                    model.uv.emplace_back(0.0f, 0.0f); // default UV
                }
            }

            // Load Indices
            for (int k = 0; k < mesh->mNumFaces; k++) {
                aiFace face = mesh->mFaces[k];
                for (GLuint index = 0; index < face.mNumIndices; index++) {
                    model.indices.push_back(face.mIndices[index]);
                }
            }

            // Load textures
            aiMaterial* material = scene->mMaterials[mesh->mMaterialIndex];
            GLuint textureCount = material->GetTextureCount(aiTextureType_DIFFUSE);
            mdlog.debug("aiTextureType_DIFFUSE : ", textureCount);

            // Load textures for this material
            for (int j = 0; j < textureCount; j++) {
                aiString str;
                material->GetTexture(aiTextureType_DIFFUSE, j, &str);
                const aiTexture* embeddedTexture = scene->GetEmbeddedTexture(str.C_Str());
                if (embeddedTexture) {
                    model.texture.colorMap = loadTextureEmbedded(embeddedTexture->pcData, embeddedTexture->mWidth);
                    mdlog.debug("Embedded texture loaded:", str.C_Str());
                }
            }

            // Add the geometry object (mesh) to the models vector
            model.setupBuffers();
            models.push_back(model);
        }

        // Recursively process each child node
        for (unsigned int i = 0; i < node->mNumChildren; i++) {
            processNode(node->mChildren[i], scene, models);
        }
    }

    // Main function to load the model
    std::vector<Geometry> loadModel(std::string const& path) {
        Assimp::Importer importer;
        std::vector<Geometry> models;

        const aiScene* scene_ = importer.ReadFile(path, aiProcess_CalcTangentSpace | aiProcess_Triangulate | aiProcess_JoinIdenticalVertices | aiProcess_SortByPType);
        if (nullptr == scene_) {
            mdlog.error("Import Failed", importer.GetErrorString());
            return models;
        }

        mdlog.debug("Number of Meshes in Scene: ", scene_->mNumMeshes);

        // Process the root node
        aiNode* rootNode = scene_->mRootNode;
        processNode(rootNode, scene_, models);

        return models;
    }

}
