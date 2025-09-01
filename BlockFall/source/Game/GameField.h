#pragma once

#include "Board.h"

struct GameField
{
    GameField() = delete;
    GameField(GameRulesetsEnum currentRuleset)
    {
        ruleset = currentRuleset;
        das = Config::getInstance().getConfigData().rulesetsMap.at(ruleset).das;
        arr = Config::getInstance().getConfigData().rulesetsMap.at(ruleset).arr;
        currentSpeed = Config::getInstance().getConfigData().rulesetsMap.at(ruleset).speedLevels[currentLevel];
    }

    const GameRuleset& getGameRulesetData() const { return Config::getInstance().getConfigData().rulesetsMap.at(ruleset); }

    GameRulesetsEnum ruleset;
    int currentLevel = 0;
    float currentSpeed = 0.0f;
    float previewWindowSize = 100.0f;
    float das; // Delay before auto-shift
    float arr; // Auto repeat rate
    SDL_FPoint scoreOrigin = { static_cast<float>(Config::getInstance().getConfigData().width) - 200.0f, 50.0f }; // hardcoded for now
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