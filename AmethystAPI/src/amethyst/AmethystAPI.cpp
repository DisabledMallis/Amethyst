#include "amethyst/AmethystAPI.h"

#include "amethyst/SymbolManager.h"

void AmethystAPI::Setup()
{
    auto symbolManager = this->GetOrCreate<SymbolManager>();

    auto offset = symbolManager->get<Symbols::MemberOffsets::ClientInstance_mMinecraftGame>();
    auto address = symbolManager->get<Symbols::MemberFunctions::ClientInstance__onStartJoinGame>();
}