#include "GameScene.h"

#include "Block.h"
#include "Renderer.h"
#include "SpriteManager.h"

GameScene::GameScene()
{
    
}

GameScene::~GameScene()
{
}

void GameScene::init()
{
    SpriteManager::getInstance();
    Renderer::getInstance();
}

void GameScene::handleInput()
{
}

void GameScene::update(float deltaTime)
{
}

void GameScene::render()
{
    Renderer::getInstance().clear();
    for (int i = 0; i < static_cast<int>(BlockTypes::Total); ++i)
    {
        auto block = SpriteManager::getInstance().getBlock(static_cast<BlockTypes>(i));
        //Test Block
        float first = i < 4 ? (float)i * 192.0 : (float)(i-4) * 192.0;
        float second = (i / 4) * 192.0;

        SDL_FRect destination = {first, second, 192, 192};
        Renderer::getInstance().drawBlock(block, destination);
        //End Test Block
    }
    Renderer::getInstance().present();
}

void GameScene::cleanup()
{
}
