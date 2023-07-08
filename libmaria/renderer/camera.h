#pragma once
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

namespace maria
{
    class camera
    {
    public:
        glm::mat4 get_matrix()
        {
            glm::vec3 direction;
            direction.x = cos(glm::radians(rotation.y - 90.0f)) * cos(glm::radians(rotation.x));
            direction.y = sin(glm::radians(rotation.x));
            direction.z = sin(glm::radians(rotation.y - 90.0f)) * cos(glm::radians(rotation.x));
            front = glm::normalize(direction);
            right = glm::normalize(glm::cross(front, up));
            glm::mat4 view = glm::lookAt(position, position + front, up);
            glm::mat4 proj = glm::perspective(glm::radians(fov), aspect, near, far);
            glm::mat4 result = proj * view;
            return result;
        }
    public:
        glm::vec3 position = glm::vec3(0);
        glm::vec3 rotation = glm::vec3(0);
        float fov = 45.0f;
        float aspect = 4.0f/3.0f;
        float near = 0.1f;
        float far = 1000.0f;
    private:
        glm::vec3 right = glm::vec3(1, 0, 0);
        glm::vec3 up = glm::vec3(0, 1, 0);
        glm::vec3 front = glm::vec3(0, 0, 1);
    };
}