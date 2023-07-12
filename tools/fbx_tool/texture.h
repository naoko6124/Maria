#pragma once

#include <fstream>
#include <iostream>
#include <string>
#include <vector>

#define STB_IMAGE_IMPLEMENTATION
#include <stb/stb_image.h>

struct texture
{
    std::string name;
    int width, height, channels;
    unsigned char *data;

    texture()
    {

    }

    texture(std::string file)
    {
        data = stbi_load(file.c_str(), &width, &height, &channels, 0);
    }

    texture(aiTexture* tex)
    {
        std::cout << "a";
        aiTexel *texel = tex->pcData;
        data = stbi_load_from_memory((const stbi_uc*)texel, tex->mWidth, &width, &height, &channels, 0);
    }

    ~texture()
    {
        stbi_image_free(data);
    }

    void output(std::string path)
    {
        std::ofstream file(path, std::ios::binary);

        size_t tsize = width * height * channels;
        file.write((const char *)&width, 4);
        file.write((const char *)&height, 4);
        file.write((const char *)&channels, 4);
        file.write((const char *)data, tsize);

        file.close();
    }
private:
    std::vector<std::string> string_split(std::string str, char separator) {
        std::vector<std::string> strings;
        int startIndex = 0, endIndex = 0;
        for (int i = 0; i <= str.size(); i++)
        {
            if (str[i] == separator || i == str.size())
            {
                endIndex = i;
                std::string temp;
                temp.append(str, startIndex, endIndex - startIndex);
                strings.push_back(temp);
                startIndex = endIndex + 1;
            }
        }
        return strings;
    }
};