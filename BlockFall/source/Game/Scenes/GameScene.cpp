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
    _currentPieceData = std::make_shared<PieceData>();
    SpriteManager::getInstance();
    Renderer::getInstance();
    _gameField = GameField();
    _gameField.board->updateBoardDimensions();
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

    const float das = _gameField.das;
    const float arr = _gameField.arr;

    bool movingRight = false, movingLeft = false;

    if (_input->isKeyPressed(SDL_SCANCODE_N))
    {
        _currentPieceData->piece->rotateCW();
    
        if (!isValidRotation(*_currentPieceData->piece, _currentPieceData->renderingPosition))
        {
            // revert
            _currentPieceData->piece->rotateCCW();
        }
        else
        {
            _currentPieceData->logicalPosition.x += _currentPieceData->piece->getCurrentDeltaOrigin().x;
            _currentPieceData->logicalPosition.y += _currentPieceData->piece->getCurrentDeltaOrigin().y;
        }
    }
    if (_input->horizontalMovementIfNotOnCooldown(SDL_SCANCODE_A, das, arr))
    {
        if (_currentPieceData->logicalPosition.x > 0)
        {
            movingLeft = true;
            _currentPieceData->logicalPosition.x -= 1;
            _currentPieceData->renderingPosition.x -= 1;
        }
    }
    if (_input->horizontalMovementIfNotOnCooldown(SDL_SCANCODE_D, das, arr))
    {
        if (_currentPieceData->logicalPosition.x < BoardConsts::s_columns - _currentPieceData->piece->getPieceArea().x)
        {
            movingRight = true;
            _currentPieceData->logicalPosition.x += 1;
            _currentPieceData->renderingPosition.x += 1;
        }
    }
    _movingHorizontally = movingLeft || movingRight;


    if (_input->isSoftDropping(SDL_SCANCODE_S))
    {
        if (canSoftDrop() && _currentPieceData->logicalPosition.y < BoardConsts::s_rows - _currentPieceData->piece->getPieceArea().y)
        {
            _currentPieceData->logicalPosition.y++;
            _currentPieceData->renderingPosition.y++;
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
            _currentPieceData->piece = std::move(_previewNextPiece);
        }
        else
        {
            // First run: generate initial piece
            PieceShapes randomShape = static_cast<PieceShapes>(dist(_gen));
            _currentPieceData->piece = std::make_shared<Piece>(randomShape);
        }
        PieceShapes randomShape = static_cast<PieceShapes>(dist(_gen));
        _previewNextPiece = std::make_shared<Piece>(randomShape);

        _currentPieceData->logicalPosition = _currentPieceData->renderingPosition = BoardConsts::s_spawnGridPosition;
        _currentPieceData->logicalPosition.y += -_currentPieceData->piece->getDeltaY();
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
            if (_currentPieceData->logicalPosition.y < BoardConsts::s_rows - _currentPieceData->piece->getPieceArea().y)
            {
                _currentPieceData->logicalPosition.y++;
                _currentPieceData->renderingPosition.y++;

                _currentPieceTimeToDrop = 0.0f;
            }
            else
            {
                _gameState = GameState::Spawning;
                _lockedSoftDrop = true;
                savePieceOnBoard();
            }
        }

    }
    else if (_gameState == GameState::Falling){
        if (_currentPieceTimeToDrop >= _configData.speedLevels[currentLevel])
        {
            if (_currentPieceData->logicalPosition.y < BoardConsts::s_rows - _currentPieceData->piece->getPieceArea().y)
            {
                _currentPieceData->logicalPosition.y++;
                _currentPieceData->renderingPosition.y++;
                _currentPieceTimeToDrop = 0.0f;
            }
            else
            {
                _gameState = GameState::Spawning;
                savePieceOnBoard();
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
    auto pieceBlocksCoord = _currentPieceData->piece->getBlocksCoord();

    Renderer::getInstance().drawPiece(pieceBlocksCoord, *_currentPieceData->piece, _gameField.board->getCellSize(), _gameField.board->convertGridPointToPixel(_currentPieceData->renderingPosition));
    Renderer::getInstance().drawPreviewWindow(_previewNextPiece->getBlocksCoord(), *_previewNextPiece, _gameField.previewWindowSize, _gameField.previewZoneOrigin);


    if (_gameField.board) {
        Renderer::getInstance().drawBoard(*_gameField.board);
    }
        Renderer::getInstance().present();
}

void GameScene::savePieceOnBoard() const
{
    _gameField.board->savePieceOnBoard(*_currentPieceData);

}

bool GameScene::isValidRotation(Piece& piece, const SDL_Point& pos) const
{
    const auto coords = piece.getBlocksCoord();
    for (const auto& block : coords)
    {
        int x = pos.x + block.x;
        int y = pos.y + block.y;

        // Check board bounds
        if (x < 0 || x >= BoardConsts::s_columns ||
            y < 0 || y >= BoardConsts::s_rows)
        {
            return false;
        }

        // Check collisions with existing blocks on board
        //if (_gameField->board->isCellOccupied(x, y))
        //{
        //    return false;
        //}
    }
    return true;
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
