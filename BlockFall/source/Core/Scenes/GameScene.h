#pragma once
#include <memory>
#include <random>

#include "Scene.h"
#include "GameField.h"
#include "InputManager.h"


class GameScene : public Scene
{
public:
    GameScene() = default;
    ~GameScene() override;


    void init() override;
    void handleInput(const float dt) override;
    void update(const float deltaTime) override;
    void cleanup() override;

    inline std::string getName() const override { return "GameScene"; }
    void handleEvents(const SDL_Event& event);
    bool shouldQuit() const;

private:
    void render() override;

    void calculateBoardOccupiedCells();

    std::unique_ptr<GameField> _gameField;
    std::unique_ptr<Piece> _currentPiece;
    std::unique_ptr<Piece> _previewNextPiece;
    std::unique_ptr<InputManager> _input;
    SDL_Point _currentPiecePosition;
    int currentLevel = 0;
    float _currentPieceTimeToDrop;
    bool _pieceFalling = false;
    std::random_device _rd;
};
