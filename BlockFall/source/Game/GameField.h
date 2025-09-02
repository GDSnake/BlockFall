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

    inline const GameRuleset& getGameRulesetData() const { return Config::getInstance().getConfigData().rulesetsMap.at(ruleset); }

    inline const Uint64 getPointsPerClearedLine(const uint8_t clearedLines) const
    {
        if (clearedLines == 0 || clearedLines > 4) {
            return 0;
        }
        return getGameRulesetData().basePointsForClearedLines[clearedLines - 1] * (currentLevel + 1);
    }

    inline const uint8_t getLinesToLevelUp() const
    {
        if (currentLevel < getGameRulesetData().linesToLevelUp.size()) {
            return getGameRulesetData().linesToLevelUp[currentLevel];
        }
        return getGameRulesetData().linesToLevelUp.back();
    }

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

    inline void levelUp()
    {
        currentLevel++;
        currentClearedLines -= getLinesToLevelUp();
    }

    inline void restartGame()
    {
        currentLevel = currentClearedLines = totalClearedLines = score = 0u;
        board->clearBoard();
    }

    GameRulesetsEnum ruleset;
    uint64_t currentLevel = 0;
    float currentSpeed = 0.0f;
    float previewWindowSize = 100.0f;
    float das; // Delay before auto-shift
    float arr; // Auto repeat rate
    SDL_FPoint scoreOrigin = { static_cast<float>(Config::getInstance().getConfigData().width) - 200.0f, 50.0f }; // hardcoded for now
    std::shared_ptr<Board> board = std::make_shared<Board>();
    SDL_FPoint previewZoneOrigin = { static_cast<float>(Config::getInstance().getConfigData().width) - 200.0f, static_cast<float>(Config::getInstance().getConfigData().height) - 200.0f }; // hardcoded for now
    uint64_t score = 0;
    uint64_t currentClearedLines = 0;
    uint64_t totalClearedLines = 0;
    bool isPaused = false;
};