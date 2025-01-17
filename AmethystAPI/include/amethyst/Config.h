#pragma once
#include "nlohmann/json.hpp"
#include "amethyst/Log.h"
#include <string>
#include <vector>
using json = nlohmann::json;

class Config {
public:
    std::string injectedMod;
    std::string gameVersion;
    std::vector<std::string> mods;
    bool promptDebugger;

public:
    Config();
    Config(std::string& text);
};