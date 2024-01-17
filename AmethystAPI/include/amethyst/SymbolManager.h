#pragma once

#include "IFeature.h"

#include "minecraft/Symbols.h"

#include <magic_enum.hpp>

#include <array>
#include <optional>
#include <unordered_map>

struct SymbolManager : public IFeature
{
    static constexpr size_t numKinds = magic_enum::enum_count<Symbols::Kind>();

    template <typename symbol_t> struct KindOf {};
    template<> struct KindOf<Symbols::VirtualOffsets> { static constexpr Symbols::Kind kind = Symbols::Kind::VirtualOffset; };
    template<> struct KindOf<Symbols::MemberOffsets> { static constexpr Symbols::Kind kind = Symbols::Kind::MemberOffset; };
    template<> struct KindOf<Symbols::MemberFunctions> { static constexpr Symbols::Kind kind = Symbols::Kind::MemberFunction; };
    template<> struct KindOf<Symbols::GlobalFunctions> { static constexpr Symbols::Kind kind = Symbols::Kind::GlobalFunction; };

    template <Symbols::Kind kind> struct KindTraits {};
    template<> struct KindTraits<Symbols::Kind::VirtualOffset> { using value_type = std::optional<size_t>; };
    template<> struct KindTraits<Symbols::Kind::MemberOffset> { using value_type = std::optional<size_t>; };
    template<> struct KindTraits<Symbols::Kind::MemberFunction> { using value_type = uintptr_t; };
    template<> struct KindTraits<Symbols::Kind::GlobalFunction> { using value_type = uintptr_t; };

    virtual ~SymbolManager() = default;
    virtual void Setup() const override;

    template <auto entry, typename symbol_t = decltype(entry), Symbols::Kind kind = KindOf<symbol_t>::kind, typename return_t = KindTraits<kind>::value_type>
    return_t get() const
    {
        constexpr size_t map = magic_enum::enum_index<kind>();
        constexpr size_t entry_id = magic_enum::enum_underlying<symbol_t>(entry);
        static_assert(map < numKinds, "You cannot get a symbol map that is not one of the defined symbol kinds");

        static auto& symbolMap = this->mSymbols.at(map);
        if(symbolMap.contains(entry_id))
        {
            return symbolMap.at(entry_id);
        }

        return {};
    }

private:
    std::array<std::unordered_map<size_t, size_t>, numKinds> mSymbols;
};