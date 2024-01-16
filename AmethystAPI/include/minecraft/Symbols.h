#pragma once

#include "magic_enum.hpp"

#include <functional>

//Simple hashing function for hashing text at compile time
constexpr size_t const_hash(char const *input) {
    return *input ? static_cast<unsigned int>(*input) + 33 * const_hash(input + 1) : 5381;
}

//Hashes the enum name so that way if the order of the enums change, they will always have the same value
#define HASHED_ENTRY(entry) entry = const_hash(#entry)

struct Symbols
{
    enum struct VirtualOffsets : size_t
    {
        HASHED_ENTRY(ClientInstance_getLocalPlayer),
    };

    enum struct MemberOffsets : size_t
    {
        HASHED_ENTRY(ClientInstance_mMinecraftGame),
        HASHED_ENTRY(ClientInstance_mMinecraft),
        HASHED_ENTRY(ClientInstance_mClientInputHandler),
        HASHED_ENTRY(ClientInstance_mItemRenderer),
        HASHED_ENTRY(ClientInstance_mGuiData),
    };

    enum struct MemberFunctions : size_t
    {
        HASHED_ENTRY(ClientInstance__onStartJoinGame)
    };

    enum struct GlobalFunctions : size_t
    {

    };
};