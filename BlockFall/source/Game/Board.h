#pragma once
#include <SDL3/SDL_rect.h>
#include <cassert>

#include "Config/Config.h"
#include "Entities/EntityStructures.h"

namespace BoardConsts
{
    static constexpr size_t s_rows = 20;
    static constexpr size_t s_columns = 10;
    static constexpr int s_hiddenRows = 2; // Rows that are hidden above the visible play area

    static constexpr size_t s_boardSize = s_rows * s_columns;
    static constexpr float s_lineThickness = 3.0f;

    static constexpr  SDL_Point s_spawnGridPosition = { 4 , 3};
}

struct Board
{
private:
    inline void savePieceOnBoard(const PieceData& pieceData)
    {
        const SDL_Point& position = pieceData.position;

        const auto& block = pieceData.piece->getBlock();
        auto coordList = pieceData.piece->getBlocksCoord();
        const SDL_Point& deltaOrigin = pieceData.piece->getCurrentDeltaOrigin();
        for (auto& coord : coordList)
        {
            SDL_Point targetPosition = { position.x - deltaOrigin.x + coord.x, position.y - deltaOrigin.y + coord.y };
            int index = convertPieceCoordToArrayIndex(targetPosition);
            
            assert(index < BoardConsts::s_boardSize && index >= 0);

            _boardContent[index] = block;
        }
        _isEmpty = false;
    }

    inline uint8_t getCountOfCompletedLinesAndClear()
    {
        uint64_t clearedLines = 0;
        int writeRow = BoardConsts::s_rows - 1; // where we copy down

        // scan rows from bottom to top
        for (int y = BoardConsts::s_rows - 1; y >= 0; --y) {
            bool fullRow = true;

            // check if row y is full
            for (int x = 0; x < BoardConsts::s_columns; ++x) {
                SDL_Point pos = { x, y };
                if (!_boardContent[convertPieceCoordToArrayIndex(pos)]) {
                    fullRow = false;
                    break;
                }
            }

            // if not full → move row down
            if (!fullRow) {
                if (writeRow != y) {
                    // copy row y → writeRow AKA the row 'falls'
                    for (int x = 0; x < BoardConsts::s_columns; ++x) {
                        SDL_Point positionToBeWritten = { x, writeRow };
                        SDL_Point positionToBeRead = { x, y };
                        _boardContent[convertPieceCoordToArrayIndex(positionToBeWritten)] = _boardContent[convertPieceCoordToArrayIndex(positionToBeRead)];
                    }
                }
                --writeRow;
            }
            else {
                ++clearedLines;
            }
        }

        // clear remaining rows at top
        for (int y = writeRow; y >= 0; --y) {
            for (int x = 0; x < BoardConsts::s_columns; ++x) {
                SDL_Point pos = { x, y };
                _boardContent[convertPieceCoordToArrayIndex(pos)] = nullptr;
            }
        }
        return clearedLines;
    }
public:

    inline uint8_t processPieceHitAndGetLinePoints(const PieceData& pieceData)
    {
        savePieceOnBoard(pieceData);
        return getCountOfCompletedLinesAndClear();
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
    inline float getBoardHeight() const { return (BoardConsts::s_rows - BoardConsts::s_hiddenRows) * (_cellSize + BoardConsts::s_lineThickness) - _cellSize * 2; }

    inline float getOriginYPositionWithHiddenRows() const
    {
        return getBoardOrigin().y + (BoardConsts::s_hiddenRows * (_cellSize + BoardConsts::s_lineThickness) - BoardConsts::s_lineThickness);
    }

    inline SDL_FPoint convertGridPointToPixel(const SDL_Point& coord) const
    {
        return { _boardTopLeftOrigin.x + (static_cast<float>(coord.x) * _cellSize - BoardConsts::s_lineThickness) , _boardTopLeftOrigin.y + (static_cast<float>(coord.y) * _cellSize - BoardConsts::s_lineThickness) };
    }

    inline int convertPieceCoordToArrayIndex(const SDL_Point& position) const
    {
        return (position.y * BoardConsts::s_columns) + position.x;
    }

    inline void clearBoard()
    {
        _boardContent.fill(nullptr);
        _isEmpty = true;
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

