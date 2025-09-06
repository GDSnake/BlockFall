#pragma once
#include <iostream>
#include <nlohmann/json.hpp>
#include <fstream>
#include <SDL3/SDL_filesystem.h>

#include "Ruleset.h"

using json = nlohmann::json;
constexpr int speedLevelsNumber = 21;


enum class FontNames : uint8_t
{
    DefaultFont = 0,
    GameBoyFont,
    Total
};

enum class GameRulesetsEnum : uint8_t
{
    GameBoy = 0,
    Total
};

static GameRulesetsEnum toGameRulesetEnum (std::string_view str)
{
    if (str == "game_boy_ruleset") return GameRulesetsEnum::GameBoy;
    return GameRulesetsEnum::Total; // Default case
}

namespace FontUtils
{
    inline const std::string to_string(FontNames name)
    {
        switch (name)
        {
            case FontNames::DefaultFont: return "defaultFont";
            case FontNames::GameBoyFont: return "gameboyFont";

            default: return "unknown";
        }
    }
}

struct ConfigData {
    std::string title = "";
    int width = 800;
    int height = 600;
    bool fullscreen = false;
    int targetFPS = 60;
    bool hasError = false;
    bool drawGrid = false;
    std::string bgmPath = "";
    std::string logoPath = "";
    std::string blocksFolder = "";

    std::unordered_map<std::string,std::string> fontsFileNameMap;
    std::unordered_map<GameRulesetsEnum, GameRuleset> rulesetsMap;
};

class Config
{
public:
    static Config& getInstance()
    {
        static Config instance;
        return instance;
    }

    Config(const Config&) = delete;
    Config& operator=(const Config&) = delete;
    Config(Config&&) = delete;
    Config& operator=(Config&&) = delete;

    const ConfigData& getConfigData()
    {
        return _data;
    }

inline void loadConfig() {
        std::string filePath = std::filesystem::current_path().generic_string();

        std::ifstream file(std::format("{}/config.json",filePath));
        if (!file)
        {
            std::cerr << "Cannot load Config File.\n";
            ConfigData errorConfig;
            errorConfig.hasError = true;
            _data = errorConfig;
            return;
        }
        json data = json::parse(file);
        ConfigData cfg;
        cfg.title = data["window"]["title"];
        cfg.width = data["window"]["width"];
        cfg.height = data["window"]["height"];
        cfg.fullscreen = data["window"]["fullscreen"];
        cfg.targetFPS = data["window"]["target_fps"];
        cfg.bgmPath = data["sound"]["bgm"];
        cfg.logoPath = data["images"]["logo"];
        cfg.blocksFolder = data["images"]["blocks_folder"];
        cfg.drawGrid = data["game"]["draw_grid"];
        auto ruleSetsArray = data["game_rules"]["ruleset_list"];
        for (int i = 0; i < ruleSetsArray.size(); ++i)
        {
            nlohmann::basic_json<> rulesetJson = data["game_rules"][ruleSetsArray[i]];
            cfg.rulesetsMap[toGameRulesetEnum(ruleSetsArray[i])] = processRuleset(cfg, rulesetJson);
        }


        for (int i = 0; i < static_cast<int>(FontNames::Total); ++i)
        {
            std::string fontName = FontUtils::to_string(static_cast<FontNames>(i));
            cfg.fontsFileNameMap[fontName] = data["fonts"][fontName];
        }

        _data = cfg;
    }
private:
    Config() = default;

    ~Config() = default;

    ConfigData _data;

    inline GameRuleset processRuleset(ConfigData& cfg, const json& rulesetJson) {
        GameRuleset ruleset;
        ruleset.das = rulesetJson["game_physics"]["das"];
        ruleset.arr = rulesetJson["game_physics"]["arr"];
        ruleset.are = rulesetJson["game_physics"]["are"];
        ruleset.lineClear = rulesetJson["game_physics"]["clear_line"];
        ruleset.softDropTime = rulesetJson["game_physics"]["soft_drop"];
        ruleset.srs = rulesetJson["game_physics"]["enabled_rules"]["srs"];
        //ruleset.holdPiece = rulesetJson["enabled_rules"]["hold_piece"];
        //ruleset.ghostPiece = rulesetJson["enabled_rules"]["ghost_piece"];
        //ruleset.comboSystem = rulesetJson["enabled_rules"]["combo_system"];
        //ruleset.backToBack = rulesetJson["enabled_rules"]["back_to_back"];
        //ruleset.tSpin = rulesetJson["enabled_rules"]["t_spin"];
        //ruleset.softDropEnabled = rulesetJson["enabled_rules"]["soft_drop"];
        ruleset.hardDropEnabled = rulesetJson["game_physics"]["enabled_rules"]["hard_drop"];
        ruleset.softDropWhileMovingHorizontal = rulesetJson["game_physics"]["enabled_rules"]["soft_drop_while_moving_horizontal"];
        ruleset.softDropPointsCountOnlyIfHits = rulesetJson["soft_drop_points_count_only_if_hits"];
        ruleset.softDropPointsPerLine = rulesetJson["soft_drop_points_per_line"];
        const auto& array = rulesetJson["game_physics"]["block_fall_speed_level"];
        ruleset.speedLevels.reserve(array.size());
        for (size_t i = 0; i < array.size(); ++i) 
        {
            if (i < array.size()) {
                ruleset.speedLevels.emplace_back(array[i].get<float>());
            }
        }
        const auto& pointsArray = rulesetJson["base_points_per_line_cleared"];
        for (size_t i = 0; i < 4; ++i) 
        {
            ruleset.basePointsForClearedLines[i] = pointsArray[i].get<uint64_t>();
        }
        const auto& linesToLevelUpArray = rulesetJson["lines_to_level_up"];
        ruleset.linesToLevelUp.reserve(linesToLevelUpArray.size());
        for (size_t i = 0; i < linesToLevelUpArray.size(); ++i) 
        {
            if (i < linesToLevelUpArray.size()) {
                ruleset.linesToLevelUp.emplace_back(linesToLevelUpArray[i].get<uint8_t>());
            }
        }
        return ruleset;
    }
};

