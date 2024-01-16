#include "amethyst/Memory.h"

#define NOMINMAX
#include <Windows.h>
#include <psapi.h>

#include <mutex>
#include <thread>

uintptr_t GetMinecraftBaseAddress()
{
    static uintptr_t mc = reinterpret_cast<uintptr_t>(GetModuleHandleA("Minecraft.Windows.exe"));
    return mc;
}

unsigned long GetMinecraftSize()
{
    HMODULE base = GetModuleHandleA("Minecraft.Windows.exe");
    if (base == nullptr) return 0;

    MODULEINFO moduleInfo;
    if (!GetModuleInformation(GetCurrentProcess(), base, &moduleInfo, sizeof(MODULEINFO))) return 0;

    return moduleInfo.SizeOfImage;
}

uintptr_t SlideAddress(uintptr_t offset)
{
    return GetMinecraftBaseAddress() + offset;
}

void GenerateSigscanMask(std::string& signature, std::vector<bool>& wildcardMask, std::vector<byte>& bytes)
{
    for (size_t i = 0; i < signature.size(); ++i) {
        char c1 = signature[i];
        if (c1 == ' ') continue;

        // Wildcards
        if (c1 == '?') {
            wildcardMask.push_back(true);
            continue;
        }

        wildcardMask.push_back(false);
        char c2 = signature[++i];

        std::string hexString(1, c1);
        hexString += c2;

        bytes.push_back((byte)std::stoi(hexString, nullptr, 16));
    }
}

uintptr_t SigScan(std::string signature)
{
    std::vector<bool> wildcardMask;
    std::vector<byte> bytes;

    GenerateSigscanMask(signature, wildcardMask, bytes);
    uintptr_t base = GetMinecraftBaseAddress();
    uintptr_t size = GetMinecraftSize();

    // Brute force algorithm
    for (uintptr_t address = base; address < base + size; address++) {
        bool failed = false;
        size_t byteIndex = 0;

        for (size_t offset = 0; offset < wildcardMask.size(); offset++) {
            if (wildcardMask[offset]) continue;

            byte expectedByte = bytes[byteIndex++];
            byte realByte = *reinterpret_cast<byte*>(address + offset);

            if (expectedByte != realByte) {
                failed = true;
                break;
            }
        }

        if (failed) continue;
        return address;
    }

    Log::Error("Sigscan failed! {:s}", signature);
    throw std::exception();
}

size_t FindOffsetOfPointer(void* _base, void* _pointer, size_t maxSearchSize)
{
    uintptr_t base = reinterpret_cast<uintptr_t>(_base);
    uintptr_t pointer = reinterpret_cast<uintptr_t>(_pointer);

    for (size_t offset = 0; offset < maxSearchSize; offset++) {
        uintptr_t ptrAtOffset = *reinterpret_cast<uintptr_t*>(base + offset);
        if (ptrAtOffset == pointer) return offset;
    }

    return SIZE_MAX;
}