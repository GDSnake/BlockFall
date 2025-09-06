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
    static constexpr SDL_Color Orange = { 255,50,0,255 };

}

namespace FontSize
{
   static constexpr int Score = 32;
   static constexpr int GameOver = 50;
   static constexpr int Help = 60;

}

namespace FontText
{
    static std::string ScoreText = "Score: ";
    static std::string LevelText = "Level: ";
    static std::string LinesText = "Lines: ";
    static std::string GameOverText = "     Game Over!\nPress T to Start again";
    static std::string PauseText = "  Game Paused\nPress P to Resume";
    static std::string HelptText = R"(Controls:
    A/D - Move Left/Right
    S - Soft Drop
    Space - Rotate Clockwise
    N - Rotate Counter Clockwise
    G - While in Pause, Toggle Grid Drawing
    Esc - Exit Game)";
    //    M - While in Pause, Mute/Unmute Music

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

static const FontData LevelFontInfo = { .baseText = FontText::LevelText, .origin = {static_cast<float>(Config::getInstance().getConfigData().width) - 100.0f, 10.0f },
                                                                                         .color = FontColor::Black,
                                                                                         .fontName = FontNames::GameBoyFont,
                                                                                         .fontSize = FontSize::Score };
static const FontData LinesFontInfo = { .baseText = FontText::LinesText, .origin = {static_cast<float>(Config::getInstance().getConfigData().width) - 100.0f, 50.0f }, .color = FontColor::Black, .fontName = FontNames::GameBoyFont, .fontSize = FontSize::Score };

static const FontData GameOverFontInfo = { .baseText = FontText::GameOverText, .origin = {static_cast<float>(Config::getInstance().getConfigData().width) * 0.5f,
                                                                                               static_cast<float>(Config::getInstance().getConfigData().height) * 0.5f },
                                                                                                .color = FontColor::Red,
                                                                                                .fontName = FontNames::GameBoyFont,
                                                                                                .fontSize = FontSize::GameOver };
static const FontData PauseFontInfo = { .baseText = FontText::PauseText, .origin = {static_cast<float>(Config::getInstance().getConfigData().width) * 0.5f,
                                                                                               static_cast<float>(Config::getInstance().getConfigData().height) * 0.6f },
                                                                                                .color = FontColor::Orange,
                                                                                                .fontName = FontNames::GameBoyFont,
                                                                                                .fontSize = FontSize::GameOver };
static const FontData HelpFontInfo = { .baseText = FontText::HelptText, .origin = {20.0f, 10.0f }, .color = FontColor::Black, .fontName = FontNames::DefaultFont, .fontSize = FontSize::Help };

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

