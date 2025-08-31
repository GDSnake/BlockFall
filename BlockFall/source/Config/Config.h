#pragma once
#include <iostream>
#include <nlohmann/json.hpp>
#include <fstream>

using json = nlohmann::json;
constexpr int speedLevelsNumber = 21;
namespace ConfigHelpers
{
    static std::string getFullPath(const std::string& filePath) {
        return (std::filesystem::path(RESOURCES_PATH) / filePath).string();
    }
}

namespace FontNames
{
    static std::string GameBoyFont = "gameboyFont";
}


struct ConfigData {
    std::string title = "";
    int width = 800;
    int height = 600;
    bool fullscreen = false;
    bool hasError = false;
    bool drawGrid = false;
    std::string bgmPath = "";
    std::string logoPath = "";
    std::string blocksFolder = "";
    std::array<float, speedLevelsNumber> speedLevels;
    //Delayed Auto Shift
    float das = 0.2f;
    //Auto Repeat Rate
    float arr = 0.1f;
    //Spawn delay
    float are = 0.03f;
    float lineClear = 1.5f;
    float softDrop = 0.05f;

    //extra rules flags
    //Super Rotation System
    bool srs = false;
    bool softDropWhileMovingHorizontal = false;
    bool hardDrop = false;
    std::unordered_map<std::string,std::string> fontsMap;
};

inline std::string getConfigFilePath(const char* path) {
    std::filesystem::path exePath = std::filesystem::absolute(path);
    return (exePath.parent_path() / "config.json").string();
}


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

    ConfigData getConfigData()
    {
        return _data;
    }

    void loadConfig(const char* path) {
        std::string filePath = getConfigFilePath(path);
        std::ifstream file(filePath);
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
        cfg.bgmPath = data["sound"]["bgm"];
        cfg.logoPath = data["images"]["logo"];
        cfg.blocksFolder = data["images"]["blocks_folder"];
        cfg.drawGrid = data["game"]["draw_grid"];
        auto array = data["game_physics"]["block_fall_speed_level"];
        cfg.das = data["game_physics"]["das"];
        cfg.arr = data["game_physics"]["arr"];
        cfg.are = data["game_physics"]["are"];
        cfg.lineClear = data["game_physics"]["clear_line"];
        cfg.softDrop = data["game_physics"]["soft_drop"];

        cfg.srs = data["game_physics"]["enabled_rules"]["srs"];
        cfg.softDropWhileMovingHorizontal = data["game_physics"]["enabled_rules"]["soft_drop_while_moving_horizontal"];
        cfg.hardDrop = data["game_physics"]["enabled_rules"]["hard_drop"];
        cfg.fontsMap[FontNames::GameBoyFont] = data["fonts"]["gameboy_font"];
        for (size_t i = 0; i < array.size(); ++i) 
        {
            cfg.speedLevels[i] = array[i].get<float>();
        }
        _data = cfg;
    }
private:
    Config() = default;

    ~Config() = default;

    ConfigData _data;
};

