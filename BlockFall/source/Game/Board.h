#pragma once

#include <SDL3/SDL_rect.h>
#include "Config.h"

#include "EntityStructures.h"

namespace BoardConsts
{
    static constexpr size_t s_rows = 20;
    static constexpr size_t s_columns = 10;

    static constexpr size_t s_boardSize = s_rows * s_columns;
    static constexpr float s_lineThickness = 3.0f;

    static constexpr  SDL_Point s_spawnGridPosition = { 4 , 1};
}

struct Board
{
    inline void savePieceOnBoard(const PieceData& piece )
    {
        const SDL_Point& position = piece.renderingPosition;
        const auto& block = piece.piece->getBlock();
        auto coordList = piece.piece->getBlocksCoord();
        for (auto& coord : coordList)
        {
            int index = ((position.y + coord.y) * BoardConsts::s_columns-1) + (position.x + coord.x);
            index = index > _boardContent.size() - 1 ? _boardContent.size() - 1 : index; // Prevent out of bounds

            _boardContent[index] = block;
        }
        _isEmpty = false;
    }

    inline void updateBoardDimensions()
    {
        updateBoardTopLeftOrigin();
        updateCellSize();
    }



    inline std::array<std::shared_ptr<Block>, BoardConsts::s_boardSize> getBoardContent() const { return _boardContent; }
    inline bool isEmpty() const { return _isEmpty; }
    inline SDL_FPoint getBoardOrigin() const { return _boardTopLeftOrigin; }
    inline float getCellSize() const { return _cellSize; }
    inline float getBoardWidth() const { return BoardConsts::s_columns * (_cellSize + BoardConsts::s_lineThickness) - _cellSize;}
    inline float getBoardHeight() const { return BoardConsts::s_rows * (_cellSize + BoardConsts::s_lineThickness) - _cellSize * 2 - BoardConsts::s_lineThickness; }
    inline SDL_FPoint convertGridPointToPixel(const SDL_Point& coord) const
    {
        return { _boardTopLeftOrigin.x + (static_cast<float>(coord.x) * _cellSize - BoardConsts::s_lineThickness) , _boardTopLeftOrigin.y + (static_cast<float>(coord.y) * _cellSize - BoardConsts::s_lineThickness) };
    }

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

    std::array<std::shared_ptr<Block>, BoardConsts::s_boardSize> _boardContent;
    bool _isEmpty = true;
    float _cellSize = 20.0f;
    SDL_FPoint _boardTopLeftOrigin = { 0.0f, 0.0f };


};

