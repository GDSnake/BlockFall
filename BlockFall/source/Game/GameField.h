#pragma once

#include "Board.h"

struct GameField
{
    float previewWindowSize = 100.0f;
    float das = Config::getInstance().getConfigData().das; // Delay before auto-shift
    float arr = Config::getInstance().getConfigData().arr;// Auto repeat rate
    std::shared_ptr<Board> board = std::make_shared<Board>();
    SDL_FPoint previewZoneOrigin = { static_cast<float>(Config::getInstance().getConfigData().width) - 200.0f, static_cast<float>(Config::getInstance().getConfigData().height) - 200.0f};
};