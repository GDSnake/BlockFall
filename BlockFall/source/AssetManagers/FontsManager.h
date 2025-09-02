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
    static constexpr SDL_Color Red = { 255,0,0,255 };

}

namespace FontSize
{
   static constexpr int Score = 32; 
}

namespace FontText
{
    static std::string ScoreText = "Score: ";
    static std::string LevelText = "Level: ";
    static std::string LinesText = "Lines: ";
    static std::string GameOverText = "Game Over!\nPress T to Start again";

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
static const FontData LevelFontInfo = { .baseText = FontText::LevelText, .origin = {Config::getInstance().getConfigData().width - 100.0f, 10.0f }, .color = FontColor::Black, .fontName = FontNames::GameBoyFont, .fontSize = FontSize::Score };
static const FontData LinesFontInfo = { .baseText = FontText::LinesText, .origin = {Config::getInstance().getConfigData().width - 100.0f, 50.0f }, .color = FontColor::Black, .fontName = FontNames::GameBoyFont, .fontSize = FontSize::Score };
static const FontData GameOverFontInfo = { .baseText = FontText::GameOverText, .origin = {Config::getInstance().getConfigData().width * 0.5f, Config::getInstance().getConfigData().height * 0.5f }, .color = FontColor::Red, .fontName = FontNames::GameBoyFont, .fontSize = FontSize::Score };

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

