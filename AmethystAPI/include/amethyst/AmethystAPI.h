#pragma once

#include "IFeature.h"

#include <cstdint>
#include <optional>
#include <memory>
#include <unordered_map>

#include "entt/core/type_info.hpp"

struct AmethystAPI
{

    void Setup();

    template <typename feature_t>
    static consteval const uint32_t Identifier()
    {
        return entt::type_hash<feature_t>::value();
    }

    template <typename feature_t>
    bool Has() const
    {
        static uint32_t id = Identifier<feature_t>();
        return this->mFeatures.contains(id);
    }

    template <typename feature_t>
    feature_t* Get() const
    {
        constexpr uint32_t id = Identifier<feature_t>();

        if(this->Has<feature_t>())
        {
            return static_cast<feature_t*>(this->mFeatures.at(id).get());
        }
        return nullptr;
    }

    template <typename feature_t>
    feature_t* GetOrCreate()
    {
        constexpr uint32_t id = Identifier<feature_t>();

        if(this->Has<feature_t>())
        {
            return this->Get<feature_t>();
        }
        this->mFeatures[id] = std::make_unique<feature_t>();
        return this->Get<feature_t>();
    }

private:
    std::unordered_map<uint32_t, std::unique_ptr<IFeature>> mFeatures;
};