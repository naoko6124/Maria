#pragma once
#include <glad/gl.h>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>
#include <renderer/shader.h>
#include <renderer/transform.h>

namespace maria
{
    class animation
    {
    public:
        struct keyframe
        {
            transform t;
            float time;
            keyframe() {}
            keyframe(transform tr, float ti)
            {
                t = tr;
                time = ti;
            }
        };
        struct channel
        {
            std::string name;
            std::vector<keyframe> keyframes;
        };

    public:
        channel *get_channel(std::string name)
        {
            for (auto &c : channels)
                if (c.name == name)
                    return &c;
            return nullptr;
        }
        static transform interpolate(keyframe t1, keyframe t2, float time)
        {
            transform result;
            float fulldistance = abs(t2.time - t1.time);
            float t2p = abs(t1.time - time) / fulldistance;
            float t1p = abs(t2.time - time) / fulldistance;
            result.position.x = t1.t.position.x * t1p + t2.t.position.x * t2p;
            result.position.y = t1.t.position.y * t1p + t2.t.position.y * t2p;
            result.position.z = t1.t.position.z * t1p + t2.t.position.z * t2p;
            result.rotation.w = t1.t.rotation.w * t1p + t2.t.rotation.w * t2p;
            result.rotation.x = t1.t.rotation.x * t1p + t2.t.rotation.x * t2p;
            result.rotation.y = t1.t.rotation.y * t1p + t2.t.rotation.y * t2p;
            result.rotation.z = t1.t.rotation.z * t1p + t2.t.rotation.z * t2p;
            return result;
        }
        void print()
        {
            for (channel &c : channels)
            {
                printf("-%s\n", c.name.c_str());
                for (keyframe k : c.keyframes)
                {
                    printf("--[%.1f] (%.1f, %.1f, %.1f) (%.1f, %.1f, %.1f) (%.1f, %.1f, %.1f)\n",
                           k.time,
                           k.t.position.x, k.t.position.y, k.t.position.z,
                           k.t.rotation.x, k.t.rotation.y, k.t.rotation.z,
                           k.t.scale.x, k.t.scale.y, k.t.scale.z);
                }
            }
        }
    public:
        std::vector<channel> channels;
        float duration;
    };
}