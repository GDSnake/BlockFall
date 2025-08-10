#pragma once
#include <string>
#include <vector>

#include "Block.h"
#include "Piece.h"

constexpr int rows = 20;
constexpr int columns = 10;

constexpr size_t boardSize = rows * columns;

struct Board
{
private:
    std::array<Block, boardSize> _board;
};

