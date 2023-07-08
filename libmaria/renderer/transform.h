#pragma once
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>
#include <glm/gtx/quaternion.hpp>

namespace maria
{
    class transform
    {
    public:
        transform() {}
        transform(glm::vec3 p, glm::quat r, glm::vec3 s)
        {
            position = p;
            rotation = r;
            scale = s;
        }
        glm::mat4 get_matrix()
        {
            glm::mat4 m_scale = glm::scale(glm::mat4(1), scale);
            glm::mat4 m_rotation = glm::toMat4(rotation);
            glm::mat4 m_position = glm::translate(glm::mat4(1), position);
            glm::mat4 model = m_position * m_rotation * m_scale;
            return model;
        }
    public:
        glm::vec3 position = glm::vec3(0);
        glm::quat rotation = glm::quat(glm::vec3(0));
        glm::vec3 scale = glm::vec3(1);
    };
}