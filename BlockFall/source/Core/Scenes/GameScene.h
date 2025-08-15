#pragma once
#include <memory>
#include <random>

#include "Scene.h"
#include "GameField.h"

class GameScene : public Scene
{
public:
    GameScene() = default;
    ~GameScene() override;


    void init() override;
    void handleInput() override;
    void update(float deltaTime) override;
    void cleanup() override;

    std::string getName() const override { return "GameScene"; }

private:
    void render() override;

    std::unique_ptr<GameField> _gameField;
    std::unique_ptr<Piece> _currentPiece;
    std::unique_ptr<Piece> _previewNextPiece;
    SDL_FPoint _currentPiecePosition;
    bool _pieceFalling = false;
    std::random_device _rd;
};