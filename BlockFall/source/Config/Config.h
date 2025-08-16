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
        cfg.blocksFolder = data["images"]["blocksFolder"];
        cfg.drawGrid = data["game"]["drawGrid"];
        auto arr = data["blockFallSpeedLevel"];
        for (size_t i = 0; i < arr.size(); ++i) 
        {
            cfg.speedLevels[i] = arr[i].get<float>();
        }
        _data = cfg;
    }
private:
    Config() = default;

    ~Config() = default;

    ConfigData _data;
};

