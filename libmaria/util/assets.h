#pragma once

#include <string>
#include <sstream>
#include <fstream>

#define STB_IMAGE_IMPLEMENTATION
#include <stb/stb_image.h>

#include <renderer/mesh.h>
#include <renderer/skinned_mesh.h>
#include <renderer/texture.h>
#include <renderer/skeleton.h>
#include <renderer/animation.h>

namespace maria
{
    class assets
    {
    public:
        struct image
        {
            int width, height, channels;
            unsigned char* data;
            image(std::string path)
            {
                data = stbi_load(path.c_str(), &width, &height, &channels, 4);
            }
            ~image()
            {
                stbi_image_free(data);
            }
        };
    public:
        static std::string load_file(std::string path)
        {
            std::ifstream file;
            file.open(path);
            std::stringstream text_stream;
            text_stream << file.rdbuf();
            file.close();
            std::string text = text_stream.str();
            return text;
        }
        static void load_mesh(mesh& m, std::string path)
        {
            std::ifstream file(path, std::ios::binary);

            char vertices_name[8];
            size_t vertices_size;
            file.read((char *)&vertices_name, 8);
            file.read((char *)&vertices_size, 8);

            std::vector<mesh::vertex> vertices;
            for (int i = 0; i < vertices_size; i++)
            {
                mesh::vertex v;
                file.read((char *)&v.position, 12);
                file.read((char *)&v.normal, 12);
                file.read((char *)&v.texcoord, 8);
                vertices.push_back(v);
            }

            char indices_name[8];
            size_t indices_size;
            file.read((char *)&indices_name, 8);
            file.read((char *)&indices_size, 8);

            std::vector<unsigned int> indices;
            for (int i = 0; i < indices_size; i++)
            {
                unsigned int index;
                file.read((char *)&index, 4);
                indices.push_back(index);
            }

            file.close();

            m.set_vertices(vertices);
            m.set_indices(indices);
        }
        static void load_skinned_mesh(skinned_mesh& m, std::string path)
        {
            std::ifstream file(path, std::ios::binary);

            char vertices_name[8];
            size_t vertices_size;
            file.read((char *)&vertices_name, 8);
            file.read((char *)&vertices_size, 8);

            std::vector<skinned_mesh::vertex> vertices;
            for (int i = 0; i < vertices_size; i++)
            {
                skinned_mesh::vertex v;
                file.read((char *)&v.position, 12);
                file.read((char *)&v.normal, 12);
                file.read((char *)&v.texcoord, 8);
                file.read((char *)&v.bone_id, 16);
                file.read((char *)&v.weight, 16);
                vertices.push_back(v);
            }

            char indices_name[8];
            size_t indices_size;
            file.read((char *)&indices_name, 8);
            file.read((char *)&indices_size, 8);

            std::vector<unsigned int> indices;
            for (int i = 0; i < indices_size; i++)
            {
                unsigned int index;
                file.read((char *)&index, 4);
                indices.push_back(index);
            }

            file.close();

            m.set_vertices(vertices);
            m.set_indices(indices);
        }
        static void load_texture(texture& tex, std::string path)
        {
            std::ifstream file(path, std::ios::binary);

            int width, height, channels;
            file.read((char *)&width, 4);
            file.read((char *)&height, 4);
            file.read((char *)&channels, 4);
            size_t texture_size = width * height * channels;
            char *data = new char[texture_size];
            file.read((char *)data, texture_size);

            tex.set_texture(data, width, height, channels);

            delete[] data;

            file.close();
        }
        static void load_skeleton(skeleton& s, std::string path)
        {
            std::ifstream file(path, std::ios::binary);

            skeleton::bone root;

            glm::mat4 global_inverse;
            file.read((char *)&global_inverse, 64);

            load_bone(root, file);

            s.global_inverse = global_inverse;
            s.root = root;

            file.close();
        }
        static animation load_animation(std::string path)
        {
            std::ifstream file(path, std::ios::binary);

            animation a;

            int chsize;
            file.read((char *)&chsize, 4);
            file.read((char *)&a.duration, 4);

            for (int i = 0; i < chsize; i++)
            {
                animation::channel c;
                char cname[12];
                file.read((char*)cname, 12);
                c.name = cname;
                int kfsize;
                file.read((char *)&kfsize, 4);

                for (int j = 0; j < kfsize; j++)
                {
                    animation::keyframe k;
                    file.read((char *)&k.t.position, 12);
                    file.read((char *)&k.t.rotation, 16);
                    file.read((char *)&k.t.scale, 12);
                    file.read((char *)&k.time, 4);
                    c.keyframes.push_back(k);
                }
                a.channels.push_back(c);
            }

            file.close();

            return a;
        }

    private:
        static void load_bone(skeleton::bone &b, std::ifstream &file)
        {
            char bname[12];
            file.read((char *)bname, 12);
            b.name = bname;
            size_t childrens;
            file.read((char *)&childrens, 4);
            file.read((char *)&b.offset, 64);

            for (int i = 0; i < childrens; i++)
            {
                skeleton::bone c;
                load_bone(c, file);
                b.children.push_back(c);
            }
        }
    };
}