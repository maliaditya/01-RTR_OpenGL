#pragma once
#include "precomp.h"

namespace OGL{
    
    Logger mdlog("Model.log");
    
    std::vector<Geometry> loadModel(std::string const& path)
    {

        Assimp::Importer importer;
        std::vector<Geometry> models;

        const aiScene* scene_ = importer.ReadFile(path, aiProcess_CalcTangentSpace | aiProcess_Triangulate | aiProcess_JoinIdenticalVertices | aiProcess_SortByPType);
        const aiString name;
        if(nullptr == scene_)
        {
            mdlog.error("Import Failed", importer.GetErrorString());
        }

        for(int i=0;i<scene_->mNumMeshes;i++ )
        {
            aiMesh* mesh = scene_->mMeshes[i];
            std::string meshName =  mesh->mName.C_Str(); // Get mesh name

            // Empty Model 
            Geometry model;

            // Add name
            model.name = meshName;
            
            // Add Vertices
            for(int j=0;j<mesh->mNumVertices;j++)
            {
                aiVector3D vertex = mesh-> mVertices[j];

                // Load position
                model.positions.emplace_back(vertex.x,  vertex.y, vertex.z);

                // Load normals
                if(mesh->HasNormals())
                {
                    aiVector3D normal = mesh->mNormals[j];
                    model.normals.emplace_back(normal.x,normal.y, normal.z);
                }    

                // Load UVs
                if(mesh->HasTextureCoords(0))
                {
                    aiVector3D uv = mesh->mTextureCoords[0][j];
                    model.uv.emplace_back(uv.x, uv.y);
                }
                else
                    model.uv.emplace_back(0.0f,0.0f);
            }

            // Prepare Indices
            for(int k=0;k<mesh->mNumFaces;k++)
            {
                aiFace face = mesh->mFaces[k];
                for(GLuint k=0;k<face.mNumIndices;k++)
                {
                    model.indices.push_back(face.mIndices[k]);
                }
            }

            mdlog.debug("Push : ",i);
            models.push_back(model);
        }
        
        return(models);
    }
}