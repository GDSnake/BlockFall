#pragma once
#include <array>
#include <vector>

struct GameRuleset
{
    //Delayed Auto Shift
    float das = 0.2f;
    //Auto Repeat Rate
    float arr = 0.1f;
    //Spawn delay
    float are = 0.03f;
    float lineClear = 1.5f;
    float softDropTime = 0.05f;

    bool srs = false; // Super Rotation System
    bool holdPiece = false; // Allow holding pieces
    bool ghostPiece = false; // Show ghost piece
    bool comboSystem = false; // Enable combo scoring
    bool backToBack = false; // Enable back-to-back bonuses
    bool tSpin = false; // Enable T-Spin mechanics
    bool softDropEnabled = false; // Enable soft drop scoring
    bool hardDropEnabled = false; // Enable hard drop scoring
    bool softDropWhileMovingHorizontal = false;

    std::vector<float> speedLevels; // Falling speed levels
    std::array<float, 4> basePointsForClearedLines;
};
