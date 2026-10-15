#include "FontsManager.h"

#include <SDL3_ttf/SDL_ttf.h>

#include "Config/Config.h"

const FontData& getLevelFontInfo()
{
    static const FontData info = { .baseText = FontText::LevelText, .origin = {static_cast<float>(Config::getInstance().getConfigData().width) - 100.0f, 10.0f },
                                   .color = FontColor::Black,
                                   .fontName = FontNames::GameBoyFont,
                                   .fontSize = FontSize::Score };
    return info;
}

const FontData& getLinesFontInfo()
{
    static const FontData info = { .baseText = FontText::LinesText, .origin = {static_cast<float>(Config::getInstance().getConfigData().width) - 100.0f, 50.0f },
                                   .color = FontColor::Black,
                                   .fontName = FontNames::GameBoyFont,
                                   .fontSize = FontSize::Score };
    return info;
}

const FontData& getGameOverFontInfo()
{
    static const FontData info = { .baseText = FontText::GameOverText, .origin = {static_cast<float>(Config::getInstance().getConfigData().width) * 0.5f,
                                                                                  static_cast<float>(Config::getInstance().getConfigData().height) * 0.5f },
                                   .color = FontColor::Red,
                                   .fontName = FontNames::GameBoyFont,
                                   .fontSize = FontSize::GameOver };
    return info;
}

const FontData& getPauseFontInfo()
{
    static const FontData info = { .baseText = FontText::PauseText, .origin = {static_cast<float>(Config::getInstance().getConfigData().width) * 0.5f,
                                                                               static_cast<float>(Config::getInstance().getConfigData().height) * 0.6f },
                                   .color = FontColor::Orange,
                                   .fontName = FontNames::GameBoyFont,
                                   .fontSize = FontSize::GameOver };
    return info;
}

TTF_Font* FontsManager::getFont(FontNames font) const
{
    return _fontsTTFMap.at(FontUtils::to_string(font));
}

FontsManager::FontsManager()
{

    for (int i = 0; i < static_cast<int>(FontNames::Total); ++i)
    {
        std::string fontName = FontUtils::to_string(static_cast<FontNames>(i));
        std::string font = Config::getInstance().getConfigData().fontsFileNameMap.at(fontName);
        _fontsTTFMap[fontName] = TTF_OpenFont(font.c_str(), 40);
        if (!_fontsTTFMap.at(fontName))
        {
            std::cerr << "Failed to load font: " << SDL_GetError() << '\n';
        }
    }
}

FontsManager::~FontsManager()
{
    for (auto& font : _fontsTTFMap | std::views::values)
    {
        if (font)
        {
            TTF_CloseFont(font);
            font = nullptr;
        }
    }
}
