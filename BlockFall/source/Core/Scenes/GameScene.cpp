#include "GameScene.h"

#include "Config.h"
#include "Renderer.h"
#include "Piece.h"
#include "SpriteManager.h"


GameScene::~GameScene()
{
}

void GameScene::init()
{
    SpriteManager::getInstance();
    Renderer::getInstance();
    _board = std::make_unique<Board>();
    _board->updateBoardDimensions();
}

void GameScene::handleInput()
{
}

void GameScene::update(float dt)
{
    render();
}

void GameScene::render()
{
    Renderer::getInstance().clear();
    SDL_SetRenderDrawColor(Renderer::getInstance().getRenderer(), 255, 255, 255, 255);

    for (int i = 0; i < (static_cast<int>(PieceShapes::Total)*2); ++i)
    {
        //Test Block
        Piece piece = Piece(static_cast<PieceShapes>(i% static_cast<int>(PieceShapes::Total)));
        auto pieceBlocks = piece.getPiece();
        float squareSize = 23.0f;
        float padding = 2.0f;
        float initialPadding = 50.0f;
        float yy = 0 + initialPadding * i;
        SDL_FPoint origin = {50.0f,yy};
        Renderer::getInstance().drawPiece(pieceBlocks, squareSize,padding,origin);
        //End Test Block
    }
    if (_board) {
        Renderer::getInstance().drawBoard(*_board);
    }
        Renderer::getInstance().present();
}

void GameScene::cleanup()
{
}
