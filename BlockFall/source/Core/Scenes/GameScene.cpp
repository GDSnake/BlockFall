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
    _input = std::make_unique<InputManager>();
}

void GameScene::handleInput(const float dt)
{
    const float das = _gameField->das;
    const float arr = _gameField->arr;
    if (_input->keyActionIfNotOnCooldown(SDL_SCANCODE_A, das, arr, dt))
    {
        if (_currentPiecePosition.x > 0)
        {
            _currentPiecePosition.x -= 1;
        }
    }
    if (_input->keyActionIfNotOnCooldown(SDL_SCANCODE_D, das, arr, dt))
    {
        if (_currentPiecePosition.x < BoardConsts::s_columns - _currentPiece->getPieceArea().x)
        {
            _currentPiecePosition.x += 1;
        }
    }
    if (_input->keyActionIfNotOnCooldown(SDL_SCANCODE_S, das, arr, dt))
    {
        if (_currentPiecePosition.y < BoardConsts::s_rows - _currentPiece->getPieceArea().y)
        {
            //_currentPiecePosition.y += 1;
        }
    }
}

void GameScene::update(const float dt)
{
    if (!_pieceFalling)
    {
        std::mt19937 gen(_rd());
        std::uniform_int_distribution<int> dist(0, (static_cast<int>(PieceShapes::Total) - 1));

        if (_previewNextPiece)
        {
            _currentPiece = std::move(_previewNextPiece);
        }
        else
        {
            // First run: generate initial piece
            PieceShapes randomShape = static_cast<PieceShapes>(dist(gen));
            _currentPiece = std::make_unique<Piece>(randomShape);
        }
        PieceShapes randomShape = static_cast<PieceShapes>(dist(gen));
        _previewNextPiece = std::make_unique<Piece>(randomShape);

        _currentPiecePosition = BoardConsts::s_spawnGridPosition;
        _pieceFalling = true;
        _currentPieceTimeToDrop = 0.0f;
    }
    else
    {
        handleInput(dt);
        _currentPieceTimeToDrop += dt;
    }

    if (_currentPieceTimeToDrop >= Config::getInstance().getConfigData().speedLevels[currentLevel])
    {
        if (_currentPiecePosition.y < BoardConsts::s_rows - _currentPiece->getPieceArea().y)
        {
            _currentPiecePosition.y += 1;
            _currentPieceTimeToDrop = 0.0f;
        }
        else
        {
            _pieceFalling = false;
            //_gameField->board->updateBoard();
        }
    }

    render();
}

void GameScene::handleEvents(const SDL_Event& event)
{
    _input->processInput(event);
}

bool GameScene::shouldQuit() const
{
    return _input->shouldQuit();
}

void GameScene::render()
{
    Renderer::getInstance().clear();
    SDL_SetRenderDrawColor(Renderer::getInstance().getRenderer(), 255, 255, 255, 255);
    auto pieceBlocks = _currentPiece->getPiece();

    Renderer::getInstance().drawPiece(pieceBlocks, _gameField->board->getCellSize(), BoardConsts::s_lineThickness, _gameField->board->convertGridPointToPixel(_currentPiecePosition));
    Renderer::getInstance().drawPreviewWindow(_previewNextPiece->getPiece(), _gameField->previewWindowSize, BoardConsts::s_lineThickness, _gameField->previewZoneOrigin);


    if (_gameField->board) {
        Renderer::getInstance().drawBoard(*_gameField->board);
    }
        Renderer::getInstance().present();
}

void GameScene::calculateBoardOccupiedCells()
{

}

void GameScene::cleanup()
{
}
