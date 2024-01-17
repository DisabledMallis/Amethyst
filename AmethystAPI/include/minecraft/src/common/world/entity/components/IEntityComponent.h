#pragma once

#include "amethyst/Utility.h"
#include <cstdint>

struct IEntityComponent
{
    using hash_t = uint32_t;

    static constexpr const char* type_name = "IEntityComponent";
    static constexpr hash_t hash = fnv1a32(type_name);
};