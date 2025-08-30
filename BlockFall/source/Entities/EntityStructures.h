#pragma once
#include <SDL3/SDL_rect.h>

#include "Piece.h"

struct PieceData
{
    std::shared_ptr<Piece> piece = nullptr;
    SDL_Point position = { -1, -1};
};
