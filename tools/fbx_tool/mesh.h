#pragma once

#include <assimp/Importer.hpp>
#include <assimp/postprocess.h>
#include <assimp/quaternion.h>
#include <assimp/scene.h>

#include <fstream>
#include <iostream>
#include <string>
#include <vector>

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/quaternion.hpp>
#include <glm/gtc/type_ptr.hpp>
#include <glm/gtx/string_cast.hpp>

struct vertex
{
    glm::vec3 position = glm::vec3(0.0f);
    glm::vec3 normal = glm::vec3(0.0f);
    glm::vec2 texcoord = glm::vec2(0.0f);
    glm::ivec4 bone_id = glm::ivec4(0);
    glm::vec4 weight = glm::vec4(0.0f);

    void add_bone_weight(int bone_idx, float bone_weight)
    {
        if (weight[0] == 0.0f)
        {
            bone_id[0] = bone_idx;
            weight[0] = bone_weight;
            return;
        }
        if (weight[1] == 0.0f)
        {
            bone_id[1] = bone_idx;
            weight[1] = bone_weight;
            return;
        }
        if (weight[2] == 0.0f)
        {
            bone_id[2] = bone_idx;
            weight[2] = bone_weight;
            return;
        }
        if (weight[3] == 0.0f)
        {
            bone_id[3] = bone_idx;
            weight[3] = bone_weight;
            return;
        }
    }
};

struct mesh
{
    std::string name;
    bool has_bones = false;
    std::vector<vertex> vertices;
    std::vector<unsigned int> indices;

    mesh(aiMesh *aimesh)
    {
        name = aimesh->mName.C_Str();

        for (unsigned int i = 0; i < aimesh->mNumVertices; i++)
        {
            vertex v;
            v.position.x = aimesh->mVertices[i].x;
            v.position.y = aimesh->mVertices[i].y;
            v.position.z = aimesh->mVertices[i].z;
            v.normal.x = aimesh->mNormals[i].x;
            v.normal.y = aimesh->mNormals[i].y;
            v.normal.z = aimesh->mNormals[i].z;
            v.texcoord.x = aimesh->mTextureCoords[0][i].x;
            v.texcoord.y = aimesh->mTextureCoords[0][i].y;
            vertices.push_back(v);
        }

        for (unsigned int i = 0; i < aimesh->mNumFaces; i++)
        {
            aiFace face = aimesh->mFaces[i];
            for (unsigned int j = 0; j < face.mNumIndices; j++)
                indices.push_back(face.mIndices[j]);
        }

        if (!aimesh->HasBones())
            return;

        has_bones = true;
        for (unsigned int i = 0; i < aimesh->mNumBones; i++)
        {
            for (unsigned int j = 0; j < aimesh->mBones[i]->mNumWeights; j++)
            {
                unsigned int vertex_id = aimesh->mBones[i]->mWeights[j].mVertexId;
                float b_weight = aimesh->mBones[i]->mWeights[j].mWeight;
                vertices[vertex_id].add_bone_weight(i, b_weight);
            }
        }
    }

    void print()
    {
        printf("Mesh: %s\n", name.c_str());
        printf("-Vertices:\n");
        for (auto &v : vertices)
        {
            printf(" (%.2f, %.2f, %.2f) (%.2f, %.2f, %.2f) (%.2f, %.2f) (%d, %d, %d, %d) (%.2f, %.2f, %.2f, %.2f)\n",
                   v.position.x, v.position.y, v.position.z,
                   v.normal.x, v.normal.y, v.normal.z, v.texcoord.x, v.texcoord.y,
                   v.bone_id[0], v.bone_id[1], v.bone_id[2], v.bone_id[3],
                   v.weight[0], v.weight[1], v.weight[2], v.weight[3]
                   );
        }
        printf("-Indices:\n");
        for (auto &i : indices)
        {
            printf("%d,", i);
        }
        printf("\n");
    }

    void output(std::string path)
    {
        std::ofstream file(path, std::ios::binary);

        size_t vsize = vertices.size();
        std::string vname = "VERTICES";
        file.write(vname.c_str(), 8);
        file.write((const char *)&vsize, 8);
        for (auto &v : vertices)
        {
            file.write((const char *)&v.position, 12);
            file.write((const char *)&v.normal, 12);
            file.write((const char *)&v.texcoord, 8);
            if (has_bones)
            {
                file.write((const char *)&v.bone_id, 16);
                file.write((const char *)&v.weight, 16);
            }
        }

        size_t isize = indices.size();
        std::string iname = "INDICES ";
        file.write(iname.c_str(), 8);
        file.write((const char *)&isize, 8);
        for (auto &i : indices)
        {
            file.write((const char *)&i, 4);
        }

        file.close();
    }
};