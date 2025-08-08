#include <iostream>
#include <nlohmann/json.hpp>
#include <fstream>

using json = nlohmann::json;

struct ConfigData {
    std::string title = "";
    int width = 800;
    int height = 600;
    bool fullscreen = false;
    bool hasError = false;
    std::string bgmPath = "";
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
            std::cerr << "Cannot load Config File." << std::endl;
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
        _data = cfg;
    }
private:
    Config() = default;

    ~Config() = default;

    ConfigData _data;
};

