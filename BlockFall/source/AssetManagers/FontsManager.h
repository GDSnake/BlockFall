#pragma once

#include <string>
#include <unordered_map>

enum class FontNames : uint8_t;
struct TTF_Font;
class FontsManager {

public:

    static FontsManager& getInstance()
    {
        static FontsManager instance;
        return instance;
    }

    TTF_Font* getFont(FontNames font) const;

private:
    FontsManager();
    ~FontsManager();

    std::unordered_map<std::string, TTF_Font*> _fontsTTFMap;
};

