#pragma once

#include <string>
#include <map>
#include <renderer/animation.h>

namespace maria
{
    struct shading
    {
        std::string name;
    };

    struct animator
    {
        std::string current;
        std::map<std::string, animation> anims;
    };
}