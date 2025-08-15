#pragma once
#include <memory>

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
};