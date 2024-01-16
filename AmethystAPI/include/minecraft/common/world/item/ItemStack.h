#pragma once
#include "minecraft/common/world/item/ItemStackBase.h"
#include "amethyst/Memory.h"

class ItemStack : public ItemStackBase {
public:
    ItemStack();

//vfuncs:
    void reinit(std::string_view name, int count, int auxValue);
    void reinit(const Item*, int count, int auxValue);
};

static_assert(sizeof(ItemStack) == 0xA0);