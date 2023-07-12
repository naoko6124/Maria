#pragma once

#include <assimp/Importer.hpp>
#include <assimp/postprocess.h>
#include <assimp/quaternion.h>
#include <assimp/scene.h>

#include <fstream>
#include <iostream>
#include <string>
#include <sstream>
#include <vector>

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/quaternion.hpp>
#include <glm/gtc/type_ptr.hpp>
#include <glm/gtx/string_cast.hpp>

std::vector<std::string> split_string(std::string input, char delimiter)
{
    std::vector<std::string> tokens;
    std::stringstream ss(input);
    std::string token;
    while (getline(ss, token, delimiter))
    {
        tokens.push_back(token);
    }
    return tokens;
}

struct keyframe
{
    glm::vec3 position;
    glm::quat rotation;
    glm::vec3 scale;
    float time;
};

struct channel
{
    std::string name;
    std::vector<keyframe> keyframes;
};

struct animation
{
    std::string name;
    std::vector<channel> channels;
    float duration = 0.0f;

    animation(aiAnimation *aianim)
    {
        name = split_string(aianim->mName.C_Str(), '|')[1];
        for (int i = 0; i < aianim->mNumChannels; i++)
            read_channel(aianim->mChannels[i]);
    }
    void print()
    {
        printf("%s\n", name.c_str());
        for (channel& c : channels)
        {
            printf("-%s\n", c.name.c_str());
            for (keyframe k : c.keyframes)
            {
                if (k.time == 0.0f)
                printf("--[%.1f] (%.1f, %.1f, %.1f) (%.1f, %.1f, %.1f) (%.1f, %.1f, %.1f)\n",
                    k.time,
                    k.position.x, k.position.y, k.position.z,
                    k.rotation.x, k.rotation.y, k.rotation.z,
                    k.scale.x, k.scale.y, k.scale.z
                );
            }
        }
    }
    void output(std::string path)
    {
        std::ofstream file(path, std::ios::binary);

        size_t chsize = channels.size();
        file.write((const char*)&chsize, 4);
        file.write((const char*)&duration, 4);

        for (auto& c : channels)
        {
            file.write(c.name.c_str(), 12);
            size_t kfsize = c.keyframes.size();
            file.write((const char*)&kfsize, 4);

            for (auto& k : c.keyframes)
            {
                file.write((const char*)&k.position, 12);
                file.write((const char*)&k.rotation, 16);
                file.write((const char*)&k.scale, 12);
                file.write((const char*)&k.time, 4);
            }
        }

        file.close();
    }

private:
    void read_channel(aiNodeAnim *aichannel)
    {
        channel c;
        c.name = aichannel->mNodeName.C_Str();
        for (int i = 0; i < aichannel->mNumPositionKeys; i++)
        {
            aiVectorKey p_key = aichannel->mPositionKeys[i];
            aiQuatKey r_key = aichannel->mRotationKeys[i];
            aiVectorKey s_key = aichannel->mScalingKeys[i];

            keyframe k;
            k.position = glm::vec3(p_key.mValue.x, p_key.mValue.y, p_key.mValue.z);
            k.scale = glm::vec3(s_key.mValue.x, s_key.mValue.y, s_key.mValue.z);
            k.rotation = glm::quat(r_key.mValue.w, r_key.mValue.x, r_key.mValue.y, r_key.mValue.z);

            k.time = p_key.mTime;

            if (k.time > duration)
                duration = k.time;

            c.keyframes.push_back(k);
        }
        channels.push_back(c);
    }
};