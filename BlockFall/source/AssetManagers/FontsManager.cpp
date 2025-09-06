#include "FontsManager.h"

#include <SDL3_ttf/SDL_ttf.h>

#include "Config/Config.h"

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
