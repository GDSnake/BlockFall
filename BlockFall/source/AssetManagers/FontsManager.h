#pragma once

#include <string>
#include <unordered_map>
#include <SDL3/SDL_pixels.h>
#include <SDL3/SDL_rect.h>

#include "Config/Config.h"

enum class FontNames : uint8_t;
struct TTF_Font;

namespace FontColor
{
    static constexpr SDL_Color Black = { 0,0,0,255 };
}

namespace FontSize
{
   static constexpr int Score = 32; 
}

namespace FontText
{
    static std::string ScoreText = "Score: ";
}

struct FontData
{
    std::string baseText;
    SDL_FPoint origin;
    SDL_Color color;
    FontNames fontName;
    int fontSize;
};

static const FontData ScoreFontInfo = {.baseText = FontText::ScoreText, .origin = {20.0f, 10.0f }, .color = FontColor::Black, .fontName = FontNames::GameBoyFont, .fontSize = FontSize::Score};

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

