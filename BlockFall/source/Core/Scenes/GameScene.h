#pragma once
#include <memory>
#include <random>

#include "Scene.h"
#include "Board.h"

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

    std::unique_ptr<Board> _board;
    std::unique_ptr<Piece> _currentPiece;
    SDL_FPoint _currentPiecePosition;
    bool _pieceFalling = false;
    std::random_device _rd;
};