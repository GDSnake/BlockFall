#pragma once

#include "Board.h"

struct GameField
{
    float previewWindowSize = 100.0f;
    float das = Config::getInstance().getConfigData().das; // Delay before auto-shift
    float arr = Config::getInstance().getConfigData().arr;// Auto repeat rate
    std::shared_ptr<Board> board = std::make_shared<Board>();
    SDL_FPoint previewZoneOrigin = { static_cast<float>(Config::getInstance().getConfigData().width) - 200.0f, static_cast<float>(Config::getInstance().getConfigData().height) - 200.0f }; // hardcoded for now

    inline bool willHitBlockOnBoard(const std::shared_ptr<PieceData>& pieceData) const
    {
        auto coordList = pieceData->piece->getBlocksCoord();
        const SDL_Point& deltaOrigin = pieceData->piece->getCurrentDeltaOrigin();
        for (auto& coord : coordList)
        {
            SDL_Point targetPosition = { pieceData->position.x - deltaOrigin.x + coord.x, pieceData->position.y - deltaOrigin.y + coord.y };
            int index = board->convertPieceCoordToArrayIndex(targetPosition);
            assert(index < BoardConsts::s_boardSize && index >= 0);
            if (board->getBoardContent()[index])
            {
                return true;
            }
        }
        return false;
    }
};