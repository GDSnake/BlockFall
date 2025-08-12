#pragma once
#include "Scene.h"


class GameScene : public Scene
{
public:
    GameScene();
    ~GameScene() override;


    void init() override;
    void handleInput() override;
    void update(float deltaTime) override;
    void render() override;
    void cleanup() override;

    std::string getName() const override { return "GameScene"; }
};