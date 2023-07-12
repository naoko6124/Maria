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

struct bone
{
    std::string name;
    glm::mat4 offset;
    std::vector<bone> children;
};

struct skeleton
{
    std::string name;
    bone root;
    glm::mat4 global_inverse;

    skeleton(aiMesh *aimesh, const aiScene *aiscene)
    {
        name = aimesh->mName.C_Str();

        auto mt = aiscene->mRootNode->mTransformation;
        global_inverse = glm::inverse(glm::mat4(
            mt.a1, mt.b1, mt.c1, mt.d1,
            mt.a2, mt.b2, mt.c2, mt.d2,
            mt.a3, mt.b3, mt.c3, mt.d3,
            mt.a4, mt.b4, mt.c4, mt.d4
        ));

        if (!aimesh->HasBones())
            return;

        std::vector<bone> bones;
        for (unsigned int i = 0; i < aimesh->mNumBones; i++)
        {
            aiBone *aibone = aimesh->mBones[i];
            bone b;
            b.name = aibone->mName.C_Str();
            aiMatrix4x4 o = aibone->mOffsetMatrix;
            b.offset = glm::mat4(
                o.a1, o.b1, o.c1, o.d1,
                o.a2, o.b2, o.c2, o.d2,
                o.a3, o.b3, o.c3, o.d3,
                o.a4, o.b4, o.c4, o.d4
            );
            bones.push_back(b);
        }
        root = bones[0];
        aiNode *airoot = aimesh->mBones[0]->mNode;
        bone_search(root, airoot, bones);
    }
    void print()
    {
        print_bone(root, "");
    }
    void output(std::string path)
    {
        std::ofstream file(path, std::ios::binary);

        file.write((const char *)&global_inverse, 64);

        output_bone(root, file);

        file.close();
    }
    void print_matrix(std::string pre, glm::mat4& m)
    {
        for (int i = 0; i < 4; i++)
        {
            char s[4] = "";
            if (m[0][i] >= 0.0f) s[0] = ' ';
            if (m[1][i] >= 0.0f) s[1] = ' ';
            if (m[2][i] >= 0.0f) s[2] = ' ';
            if (m[3][i] >= 0.0f) s[3] = ' ';
            printf("%s[%c%.1f, %c%.1f, %c%.1f, %c%.1f]\n", pre.c_str(),
                s[0], m[0][i], s[1], m[1][i], s[2], m[2][i], s[3], m[3][i]);
        }
    }
private:
    void bone_search(bone &b, aiNode *n, std::vector<bone> &bns)
    {
        for (int i = 0; i < n->mNumChildren; i++)
        {
            for (bone &c : bns)
            {
                if (c.name == n->mChildren[i]->mName.C_Str())
                {
                    bone_search(c, n->mChildren[i], bns);
                    b.children.push_back(c);
                }
            }
        }
    }
    void print_bone(bone &b, std::string level)
    {
        printf("%s%s\n", level.c_str(), b.name.c_str());
        printf("%soffset\n", level.c_str());
        print_matrix(level, b.offset);
        level.append("- ");
        for (bone &c : b.children)
        {
            print_bone(c, level);
        }
    }
    void output_bone(bone& b, std::ofstream& file)
    {
        file.write(b.name.c_str(), 12);
        size_t childrens = b.children.size();
        file.write((const char *)&childrens, 4);
        file.write((const char *)&b.offset, 64);
        for (auto &c : b.children)
        {
            output_bone(c, file);
        }
    }
};