#pragma once

#include <cstdint>
#include <string>

/**
 * Returns the path to the amethyst UWP folder
*/
std::string GetAmethystFolder();

/*
 * Compile-time fnv1a32 hash function, this is used by EnTT for type hashes
 */
// Source: https://github.com/SRombauts/cpp-algorithms/blob/07451e149dd9c305d03c53d9b69545edd613bf82/src/algo/hash.cpp#L39
consteval uint32_t fnv1a32(const char* str)
{
    uint32_t hash = 2166136261U; // 32 bit offset_basis = 2166136261U

    for (uint32_t idx = 0; str[idx] != 0; ++idx) {
        // 32 bit FNV_prime = 224 + 28 + 0x93 = 16777619
        hash = 16777619U * (hash ^ static_cast<unsigned char>(str[idx]));
    }

    return hash;
}