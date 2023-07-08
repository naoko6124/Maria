#pragma once
#include <string>
#include <vector>
#include <glad/gl.h>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>
#include <glm/gtx/quaternion.hpp>
#include <glm/gtx/matrix_decompose.hpp>
#include <renderer/shader.h>
#include <renderer/animation.h>

namespace maria
{
    class skeleton
    {
    public:
        struct bone
        {
            std::string name;
            glm::mat4 offset;
            std::vector<bone> children;

            glm::mat4 bone_transform;
            glm::mat4 final_transform;
        };
    public:
        skeleton()
        {
            
        }
        skeleton(bone& root_bone, glm::mat4 global_inverse_matrix)
        {
            root = root_bone;
            global_inverse = global_inverse_matrix;
        }
        void set_animation(animation& anim, float time)
        {
            float cuttime = time - floor(time / anim.duration) * anim.duration;
            set_bone_animation(root, anim, cuttime);
        }
        void apply(shader& s)
        {
            int b_idx = 0;
            calculate_bone(root, glm::mat4(1.0f));
            upload_bone(root, s, b_idx);
        }
        void print()
        {
            print_bone(root, "");
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
        void print_bone(bone &b, std::string level)
        {
            printf("%s%s\n", level.c_str(), b.name.c_str());
            print_matrix(level, b.final_transform);
            level.append("- ");
            for (bone &c : b.children)
            {
                print_bone(c, level);
            }
        }
        void upload_bone(bone& b, shader& s, int& b_idx)
        {
            s.upload_mat4("bones[" + std::to_string(b_idx) + "]", b.final_transform);
            b_idx++;
            for (bone& c : b.children)
                upload_bone(c, s, b_idx);
        }
        void calculate_bone(bone& b, glm::mat4 parent)
        {
            glm::mat4 world_transform = parent * b.bone_transform;

            b.final_transform = glm::mat4(1.0f)
            * global_inverse
            * world_transform
            * b.offset
            ;

            for (bone& c : b.children)
                calculate_bone(c, world_transform);
        }
        void set_bone_animation(bone& b, animation& a, float time)
        {
            auto* c = a.get_channel(b.name);
            if (c != nullptr)
            {
                int index = 1;
                animation::keyframe kf;
                while (index < c->keyframes.size())
                {
                    kf = c->keyframes[index];
                    if (time < kf.time)
                    {
                        b.bone_transform = animation::interpolate(c->keyframes[index - 1], kf, time).get_matrix();
                        break;
                    }
                    index++;
                }
            }
            for (bone& c : b.children)
                set_bone_animation(c, a, time);
        }
    public:
        bone root;
        glm::mat4 global_inverse;
    };
}