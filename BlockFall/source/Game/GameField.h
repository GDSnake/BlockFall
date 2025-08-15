#pragma once

#include "Board.h"

struct GameField
{
    float previewWindowSize = 100.0f;
    std::unique_ptr<Board> board = std::make_unique<Board>();
    SDL_FPoint previewZoneOrigin = { Config::getInstance().getConfigData().width - 200.0f, Config::getInstance().getConfigData().height - 200.0f};
};