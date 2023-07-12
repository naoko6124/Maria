#include <assimp/Importer.hpp>
#include <assimp/scene.h>
#include <assimp/postprocess.h>
#include <assimp/quaternion.h>

#include <filesystem>
#include <iostream>
#include <fstream>
#include <string>
#include <vector>

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>
#include <glm/gtc/quaternion.hpp>
#include <glm/gtx/string_cast.hpp>

#include "mesh.h"
#include "texture.h"
#include "skeleton.h"
#include "animation.h"

namespace fs = std::filesystem;

int main(int argc, char **argv)
{
    if (argc != 2)
    {
        printf("fbx_tool model.fbx");
        return 0;
    }

    fs::path path = argv[1];

    Assimp::Importer importer;
    const aiScene *scene = importer.ReadFile(path.string().c_str(),
                                             aiProcess_CalcTangentSpace |
                                                 aiProcess_Triangulate |
                                                 aiProcess_FlipUVs |
                                                 aiProcess_JoinIdenticalVertices |
                                                 aiProcess_SortByPType |
                                                 aiProcess_PopulateArmatureData);

    fs::path p = path.parent_path();
    p.append(path.stem().string());
    fs::create_directory(p);
    path = p;

    for (int i = 0; i < scene->mNumMeshes; i++)
    {
        {
            mesh m(scene->mMeshes[i]);
            fs::path out = path;
            out.append("meshes");
            fs::create_directory(out);
            std::string file = path.stem().string();
            file.append(std::to_string(i));
            file.append(".mesh");
            out.append(file);
            m.output(out.string());
        }
        if (scene->mMeshes[i]->HasBones())
        {
            skeleton s(scene->mMeshes[i], scene);
            fs::path out = path;
            out.append("meshes");
            fs::create_directory(out);
            std::string file = path.stem().string();
            file.append(std::to_string(i));
            file.append(".skeleton");
            out.append(file);
            s.output(out.string());
        }
    }

    for (int i = 0; i < scene->mNumTextures; i++)
    {
        texture t(scene->mTextures[i]);
        fs::path out = path;
        out.append("textures");
        fs::create_directory(out);
        std::string file = path.stem().string();
        file.append(std::to_string(i));
        file.append(".texture");
        out.append(file);
        t.output(out.string());
    }

    for (int i = 0; i < scene->mNumAnimations; i++)
    {
        animation a(scene->mAnimations[i]);
        fs::path out = path;
        out.append("animations");
        fs::create_directory(out);
        std::string file = path.stem().string();
        file.append(std::to_string(i));
        file.append(".animation");
        out.append(file);
        a.output(out.string());
    }

    return 0;
}