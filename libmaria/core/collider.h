#pragma once

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

#include <renderer/transform.h>

namespace maria
{
    class collider
    {
    public:
        struct intersect_data
        {
            bool intersect;
            float distance;
        };

    public:
        intersect_data intersect(const collider &other)
        {
            glm::vec3 hs1 = size;
            hs1 *= 0.5f;

            glm::vec3 min1 = tf->position - hs1;
            glm::vec3 max1 = tf->position + hs1;

            glm::vec3 hs2 = other.size;
            hs2 *= 0.5f;

            glm::vec3 min2 = other.tf->position - hs2;
            glm::vec3 max2 = other.tf->position + hs2;

            glm::vec3 d1 = min2 - max1;
            glm::vec3 d2 = min1 - max2;
            glm::vec3 ds = glm::max(d1, d2);

            float d = ds.x;
            if (ds.y > d)
                d = ds.y;
            if (ds.z > d)
                d = ds.z;

            intersect_data dt;

            dt.distance = d;
            dt.intersect = d < 0;

            return dt;
        }
        bool operator==(const collider &other)
        {
            if (tf == other.tf && size == other.size)
                return true;
            return false;
        }

    public:
        transform *tf;
        glm::vec3 size;
    };
}