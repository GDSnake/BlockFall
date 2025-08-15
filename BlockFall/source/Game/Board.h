#pragma once

#include <vector>
#include <SDL3/SDL_rect.h>
#include "Config.h"

#include "Piece.h"

namespace BoardConsts
{
    static constexpr size_t s_rows = 20;
    static constexpr size_t s_columns = 10;

    static constexpr size_t s_boardSize = s_rows * s_columns;
    static constexpr float s_lineThickness = 3.0f;

}

struct Board
{
    inline void updateBoard(const std::vector<std::tuple<int, std::shared_ptr<Block>>>& updateBlocks)
    {
        for (auto blockTuple : updateBlocks)
        {
            _board[std::get<0>(blockTuple)] = std::get<1>(blockTuple);
        }
    }

    inline void updateBoardDimensions()
    {
        updateBoardTopLeftOrigin();
        updateCellSize();
    }



    inline std::array<std::shared_ptr<Block>, BoardConsts::s_boardSize> getBoard() { return _board; }
    inline SDL_FPoint getBoardOrigin() const { return _boardTopLeftOrigin; }
    inline float getCellSize() const { return _cellSize; }
    inline float getBoardWidth() const { return BoardConsts::s_columns * (_cellSize + BoardConsts::s_lineThickness) - _cellSize;}
    inline float getBoardHeight() const { return BoardConsts::s_rows * (_cellSize + BoardConsts::s_lineThickness) - _cellSize * 2 - BoardConsts::s_lineThickness; }

private:
    inline void updateCellSize()
    {
        float windowHeight = static_cast<float>(Config::getInstance().getConfigData().height) * 0.7f;
        _cellSize = windowHeight / BoardConsts::s_rows;

        

    }

    inline void updateBoardTopLeftOrigin()
    {
        SDL_FPoint windowSize = { static_cast<float>(Config::getInstance().getConfigData().width),static_cast<float>(Config::getInstance().getConfigData().height) };

        _boardTopLeftOrigin = { (windowSize.x * 0.3f), (windowSize.y * 0.2f) };
    }

    std::array<std::shared_ptr<Block>, BoardConsts::s_boardSize> _board;
    float _cellSize = 20.0f;
    SDL_FPoint _boardTopLeftOrigin = { 0.0f, 0.0f };


};

