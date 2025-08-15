#include "GameScene.h"

#include <random>

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
    _gameField = std::make_unique<GameField>();
    _gameField->board->updateBoardDimensions();
}

void GameScene::handleInput()
{
}

void GameScene::update(float dt)
{
    if (!_pieceFalling)
    {
        std::mt19937 gen(_rd());
        std::uniform_int_distribution<int> dist(0, (static_cast<int>(PieceShapes::Total) - 1));
        PieceShapes randomShape = static_cast<PieceShapes>(dist(gen));
        _currentPiece = std::make_unique<Piece>(randomShape);
        _currentPiecePosition = _gameField->board->getSpawnPoint();
        randomShape = static_cast<PieceShapes>(dist(gen));
        _previewNextPiece = std::make_unique<Piece>(randomShape);
        _pieceFalling = true;
    }
    render();
}

void GameScene::render()
{
    Renderer::getInstance().clear();
    SDL_SetRenderDrawColor(Renderer::getInstance().getRenderer(), 255, 255, 255, 255);

    /*for (int i = 0; i < (static_cast<int>(PieceShapes::Total)*2); ++i)
    {
        //Test Block
        Piece piece = Piece(static_cast<PieceShapes>(i % static_cast<int>(PieceShapes::Total)));
        auto pieceBlocks = piece.getPiece();
        float squareSize = 23.0f;
        float padding = 2.0f;
        float initialPadding = 50.0f;
        float yy = 0 + initialPadding * i;
        SDL_FPoint origin = {50.0f,yy};
        Renderer::getInstance().drawPiece(pieceBlocks, squareSize,padding,origin);
        //End Test Block
    }*/

    auto pieceBlocks = _currentPiece->getPiece();

    Renderer::getInstance().drawPiece(pieceBlocks, _gameField->board->getCellSize(), BoardConsts::s_lineThickness, _currentPiecePosition);
    Renderer::getInstance().drawPreviewWindow(_previewNextPiece->getPiece(), _gameField->previewWindowSize, BoardConsts::s_lineThickness, _gameField->previewZoneOrigin);


    if (_gameField->board) {
        Renderer::getInstance().drawBoard(*_gameField->board);
    }
        Renderer::getInstance().present();
}

void GameScene::cleanup()
{
}
