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
    _gameState = GameState::Starting;
    SpriteManager::getInstance();
    Renderer::getInstance();
    _gameField = std::make_unique<GameField>();
    _gameField->board->updateBoardDimensions();
    _input = std::make_unique<InputManager>();
    _gen = std::mt19937(_rd());


}

void GameScene::handleInput(const float dt)
{
#if DEBUG_BUILD

    if (_input->isKeyPressed(SDL_SCANCODE_1))
    {
        _previewNextPiece = std::make_shared<Piece>(PieceShapes::I_shape);
    }
    if (_input->isKeyPressed(SDL_SCANCODE_2))
    {
        _previewNextPiece = std::make_shared<Piece>(PieceShapes::J_shape);
    }
    if (_input->isKeyPressed(SDL_SCANCODE_3))
    {
        _previewNextPiece = std::make_shared<Piece>(PieceShapes::L_shape);
    }
    if (_input->isKeyPressed(SDL_SCANCODE_4))
    {
        _previewNextPiece = std::make_shared<Piece>(PieceShapes::S_shape);
    }
    if (_input->isKeyPressed(SDL_SCANCODE_5))
    {
        _previewNextPiece = std::make_shared<Piece>(PieceShapes::Z_shape);
    }
    if (_input->isKeyPressed(SDL_SCANCODE_6))
    {
        _previewNextPiece = std::make_shared<Piece>(PieceShapes::T_shape);
    }
        

#endif // DEBUG_BUILD

    const float das = _gameField->das;
    const float arr = _gameField->arr;

    bool movingRight = false, movingLeft = false;

    if (_input->isKeyPressed(SDL_SCANCODE_N))
    {

        _currentPiece->rotateCW();
    }
    if (_input->horizontalMovementIfNotOnCooldown(SDL_SCANCODE_A, das, arr))
    {
        if (_currentPiecePosition.x > 0)
        {
            movingLeft = true;
            _currentPiecePosition.x -= 1;
        }
    }
    if (_input->horizontalMovementIfNotOnCooldown(SDL_SCANCODE_D, das, arr))
    {
        if (_currentPiecePosition.x < BoardConsts::s_columns - _currentPiece->getPieceArea().x)
        {
            movingRight = true;
            _currentPiecePosition.x += 1;
        }
    }
    _movingHorizontally = movingLeft || movingRight;


    if (_input->isSoftDropping(SDL_SCANCODE_S))
    {
        if (canSoftDrop() && _currentPiecePosition.y < BoardConsts::s_rows - _currentPiece->getPieceArea().y)
        {
            _currentPiecePosition.y += 1;
            _gameState = GameState::SoftDrop;
            _currentPieceTimeToDrop = 0.0f;
        }
    }
    else if (_gameState != GameState::Starting && _gameState != GameState::Spawning)
    {
        _gameState = GameState::Falling;
        _lockedSoftDrop = false;
    }
}

void GameScene::update(const float dt)
{
    handleInput(dt);
    _input->update(dt);
    if (canSelectNewPiece())
    {
        std::uniform_int_distribution<int> dist(0, (static_cast<int>(PieceShapes::Total) - 1));

        if (_previewNextPiece)
        {
            _currentPiece = std::move(_previewNextPiece);
        }
        else
        {
            // First run: generate initial piece
            PieceShapes randomShape = static_cast<PieceShapes>(dist(_gen));
            _currentPiece = std::make_shared<Piece>(randomShape);
        }
        PieceShapes randomShape = static_cast<PieceShapes>(dist(_gen));
        _previewNextPiece = std::make_shared<Piece>(randomShape);

        _currentPiecePosition = BoardConsts::s_spawnGridPosition;
        _currentPieceTimeToDrop = 0.0f;
        _gameState = GameState::Falling;
    }
    else
    {
        _currentPieceTimeToDrop += dt;
    }

    if (_gameState == GameState::SoftDrop)
    {
        if (_currentPieceTimeToDrop >= _configData.softDrop)
        {
            if (_currentPiecePosition.y < BoardConsts::s_rows - _currentPiece->getPieceArea().y)
            {
                _currentPiecePosition.y += 1;
                _currentPieceTimeToDrop = 0.0f;
            }
            else
            {
                _gameState = GameState::Spawning;
                _lockedSoftDrop = true;
                //_gameField->board->updateBoard();
            }
        }

    }
    else if (_gameState == GameState::Falling){
        if (_currentPieceTimeToDrop >= _configData.speedLevels[currentLevel])
        {
            if (_currentPiecePosition.y < BoardConsts::s_rows - _currentPiece->getPieceArea().y)
            {
                _currentPiecePosition.y += 1;
                _currentPieceTimeToDrop = 0.0f;
            }
            else
            {
                _gameState = GameState::Spawning;
                //_gameField->board->updateBoard();
            }
        }
    }

    render();
}

void GameScene::handleEvents(const SDL_Event& event) const
{
    _input->handleEvent(event);
}

bool GameScene::shouldQuit() const
{
    return _input->shouldQuit();
}

void GameScene::render()
{
    Renderer::getInstance().clear();
    SDL_SetRenderDrawColor(Renderer::getInstance().getRenderer(), 255, 255, 255, 255);
    auto pieceBlocksCoord = _currentPiece->getBlocksCoord();

    Renderer::getInstance().drawPiece(pieceBlocksCoord, _currentPiece, _gameField->board->getCellSize(), _gameField->board->convertGridPointToPixel(_currentPiecePosition));
    Renderer::getInstance().drawPreviewWindow(_previewNextPiece->getBlocksCoord(), _previewNextPiece, _gameField->previewWindowSize, _gameField->previewZoneOrigin);


    if (_gameField->board) {
        Renderer::getInstance().drawBoard(*_gameField->board);
    }
        Renderer::getInstance().present();
}

void GameScene::calculateBoardOccupiedCells()
{

}

bool GameScene::canSoftDrop() const
{
    return !_movingHorizontally && !_lockedSoftDrop && _gameState != GameState::SoftDrop && _gameState != GameState::Starting;
}

bool GameScene::canSelectNewPiece() const
{
    return _gameState == GameState::Starting || _gameState == GameState::Spawning;
}

void GameScene::cleanup()
{
}
