#pragma once

#include <vector>

#include "Piece.h"

static constexpr size_t rows = 20;
static constexpr size_t columns = 10;

constexpr size_t boardSize = rows * columns;

struct Board
{
    inline void updateBoard(const std::vector<std::tuple<int, std::shared_ptr<Block>>>& updateBlocks)
    {
        for (auto blockTuple : updateBlocks)
        {
            _board[std::get<0>(blockTuple)] = std::get<1>(blockTuple);
        }
    }

    std::array<std::shared_ptr<Block>, boardSize> _board;
};

