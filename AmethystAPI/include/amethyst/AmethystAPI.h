#pragma once

#include "IFeature.h"
#include "Utility.h"

#include <cstdint>
#include <optional>
#include <typeinfo>
#include <unordered_map>

struct AmethystAPI
{
    template <typename feature_t>
    consteval uint32_t identifier() const
    {
        static uint32_t hash = fnv1a32(typeid(feature_t).name());
        return hash;
    }

    template <typename feature_t>
    bool has() const
    {
        static uint32_t id = identifier<feature_t>();
        return this->mFeatures.contains(id);
    }

    template <typename feature_t>
    std::optional<feature_t&> get() const
    {
        static uint32_t id = identifier<feature_t>();
        if(this->has<feature_t>())
        {
            return this->mFeatures.at(id);
        }
        return std::nullopt;
    }

    template <typename feature_t>
    std::optional<feature_t&> get_or_create() const
    {
        static uint32_t id = identifier<feature_t>();
        if(this->has<feature_t>())
        {
            return this->get<feature_t>();
        }
        this->mFeatures[id] = std::make_unique<feature_t>();
    }

private:
    std::unoredered_map<uint32_t, std::unique_ptr<IFeature>> mFeatures;
};