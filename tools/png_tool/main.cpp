#include <iostream>
#include <fstream>
#include <yaml-cpp/yaml.h>
#define STB_IMAGE_IMPLEMENTATION
#include <stb/stb_image.h>

int main(int argc, char** argv)
{
    if (argc != 2)
    {
        printf("png_tool image.png");
        return 0;
    }

    std::string path = argv[1];
    std::string outpath = path.substr(0, path.size() - 4);
    outpath.append(".texture");

    YAML::Node image;
    
    int width, height, channels;
    unsigned char* data = stbi_load(path.c_str(), &width, &height, &channels, 0);

    image["width"] = width;
    image["height"] = height;
    image["channels"] = channels;
    image["data"] = YAML::Binary(data, width * height * channels);

    stbi_image_free(data);

    std::ofstream file_out(outpath.c_str());
    file_out << image;

    return 0;
}