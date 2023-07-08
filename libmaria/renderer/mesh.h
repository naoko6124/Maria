#pragma once
#include <vector>
#include <glad/gl.h>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

namespace maria
{
    class mesh
    {
    public:
        struct vertex
        {
            glm::vec3 position;
            glm::vec3 normal;
            glm::vec2 texcoord;

            vertex() {}
            vertex(glm::vec3 p, glm::vec3 n, glm::vec2 t)
            {
                position = p;
                normal = n;
                texcoord = t;
            }
        };
    public:
        mesh()
        {
            glGenVertexArrays(1, &vertex_array);
        }
        ~mesh()
        {
            glDeleteVertexArrays(1, &vertex_array);
        }
        void set_vertices(const std::vector<vertex>& vertices)
        {
            glBindVertexArray(vertex_array);
            unsigned int vertex_buffer;
            glGenBuffers(1, &vertex_buffer);
            glBindBuffer(GL_ARRAY_BUFFER, vertex_buffer);
            glBufferData(GL_ARRAY_BUFFER, vertices.size() * sizeof(vertex), vertices.data(), GL_STATIC_DRAW);
            glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, sizeof(vertex), (void*)offsetof(vertex, position));
            glEnableVertexAttribArray(0);
            glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, sizeof(vertex), (void*)offsetof(vertex, normal));
            glEnableVertexAttribArray(1);
            glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, sizeof(vertex), (void*)offsetof(vertex, texcoord));
            glEnableVertexAttribArray(2);
            glBindVertexArray(0);
            glBindBuffer(GL_ARRAY_BUFFER, 0);
            glDeleteBuffers(1, &vertex_buffer);
        }
        void set_indices(const std::vector<uint32_t>& indices)
        {
            size = indices.size();
            glBindVertexArray(vertex_array);
            unsigned int index_buffer;
            glGenBuffers(1, &index_buffer);
            glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, index_buffer);
            glBufferData(GL_ELEMENT_ARRAY_BUFFER, indices.size() * sizeof(uint32_t), indices.data(), GL_STATIC_DRAW);
            glBindVertexArray(0);
            glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);
            glDeleteBuffers(1, &index_buffer);
        }
        void render()
        {
            glBindVertexArray(vertex_array);
            glDrawElements(GL_TRIANGLES, size, GL_UNSIGNED_INT, 0);
        }
    private:
        unsigned int vertex_array;
        unsigned int size;
    };
}