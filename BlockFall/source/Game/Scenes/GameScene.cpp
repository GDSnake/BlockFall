#include "GameScene.h"

#include <random>
#include <SDL3_ttf/SDL_ttf.h>

#include "AssetManagers/FontsManager.h"
#include "Config/Config.h"
#include "Core/Renderer.h"
#include "Entities/Piece.h"
#include "AssetManagers/SpriteManager.h"
#include "Entities/EntityStructures.h"
#include "Game/GameField.h"

static void applyOriginDeltaToPosition(std::shared_ptr<PieceData>& pieceData, bool toIncrement)
{
    int multiply = toIncrement ? 1 : -1;
    pieceData->position.x += pieceData->piece->getCurrentDeltaOrigin().x * multiply;
    pieceData->position.y += pieceData->piece->getCurrentDeltaOrigin().y * multiply;
}

GameScene::GameScene(GameRulesetsEnum ruleset): _gameField(ruleset)
{
    generateRandomEngine();
}

GameScene::~GameScene()
{
}

void GameScene::init()
{
    _gameState = GameState::Starting;
    _currentPieceData = std::make_shared<PieceData>();
    SpriteManager::getInstance();
    Renderer::getInstance();
    _gameField.board->updateBoardDimensions();
    _input = std::make_unique<InputManager>();
    
    _scoreText = std::format("{}{}",ScoreFontInfo.baseText, _score);
    _scoreFont = FontsManager::getInstance().getFont(ScoreFontInfo.fontName);
}

void GameScene::handleInput()
{
///////////////////////////////////
/// Handle Debug Input
///////////////////////////////////
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
    if (_input->isKeyPressed(SDL_SCANCODE_Q))
    {
        _freezeFall = !_freezeFall;
    }
    if (_input->isKeyPressed(SDL_SCANCODE_C))
    {
        _gameField.board->clearBoard();
    }
#endif // DEBUG_BUILD
// Handle Debug Input

    const float das = _gameField.das;
    const float arr = _gameField.arr;

    bool movingRight = false, movingLeft = false;

    ///////////////////////////////////
    /// Handle Input
    ///////////////////////////////////

    if (_input->isKeyPressed(SDL_SCANCODE_SPACE))
    {
        applyOriginDeltaToPosition(_currentPieceData, false);
        _currentPieceData->piece->rotateCW();
        applyOriginDeltaToPosition(_currentPieceData, true);

        if (!isValidRotation())
        {
            // revert
            applyOriginDeltaToPosition(_currentPieceData, false);
            _currentPieceData->piece->rotateCCW();
            applyOriginDeltaToPosition(_currentPieceData, true);
        }
    }

    if (_input->isKeyPressed(SDL_SCANCODE_N))
    {
        applyOriginDeltaToPosition(_currentPieceData, false);
        _currentPieceData->piece->rotateCCW();
        applyOriginDeltaToPosition(_currentPieceData, true);

        if (!isValidRotation())
        {
            // revert
            applyOriginDeltaToPosition(_currentPieceData, false);
            _currentPieceData->piece->rotateCW();
            applyOriginDeltaToPosition(_currentPieceData, true);
        }
    }

    if (_input->isKeyPressed(SDL_SCANCODE_ESCAPE))
    {
        _input->quit();
    }

    if (_input->horizontalMovementIfNotOnCooldown(SDL_SCANCODE_A, das, arr))
    {
        if (_currentPieceData->position.x > 0)
        {
            movingLeft = true;
            _currentPieceData->position.x -= 1;
            if (_gameField.willHitBlockOnBoard(_currentPieceData))
            {
                // Check if moving left caused a collision, if so revert
                _currentPieceData->position.x += 1;
            }
        }
    }
    if (_input->horizontalMovementIfNotOnCooldown(SDL_SCANCODE_D, das, arr))
    {
        if (_currentPieceData->position.x < BoardConsts::s_columns - _currentPieceData->piece->getPieceArea().x)
        {
            movingRight = true;
            _currentPieceData->position.x += 1;
            if (_gameField.willHitBlockOnBoard(_currentPieceData))
            {
                // Check if moving right caused a collision, if so revert
                _currentPieceData->position.x -= 1;
            }
        }
    }
    _movingHorizontally = movingLeft || movingRight;


    if (_input->isSoftDropping(SDL_SCANCODE_S))
    {
        if (canSoftDrop() && _currentPieceData->position.y < BoardConsts::s_rows - _currentPieceData->piece->getPieceArea().y)
        {
            _currentPieceData->position.y++;
            _gameState = GameState::SoftDrop;
            _currentPieceTimeToDrop = 0.0f;
        }
    }
    else if (_gameState != GameState::Starting && _gameState != GameState::Spawning)
    {
        _gameState = GameState::Falling;
        _lockedSoftDrop = false;
    }
// Handle Input
}

void GameScene::update(const float deltaTime)
{
    handleInput();
    _input->update(deltaTime);

    gameplayStateLogic(deltaTime);

    _scoreText = std::format("{}{}", ScoreFontInfo.baseText, _score);


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

    SDL_Point position = {_currentPieceData->position.x - _currentPieceData->piece->getCurrentDeltaOrigin().x, _currentPieceData->position.y - _currentPieceData->piece->getCurrentDeltaOrigin().y};
    Renderer::getInstance().drawPiece(pieceBlocksCoord, *_currentPieceData->piece, _gameField.board->getCellSize(), _gameField.board->convertGridPointToPixel(position), _gameField.board->getOriginYPositionWithHiddenRows());
    Renderer::getInstance().drawPreviewWindow(_previewNextPiece->getBlocksCoord(), *_previewNextPiece, _gameField.previewWindowSize, _gameField.previewZoneOrigin);


    if (_gameField.board) {
        Renderer::getInstance().drawBoard(*_gameField.board);
    }

    Renderer::getInstance().drawText(_scoreFont, _scoreText, ScoreFontInfo.origin, ScoreFontInfo.color );
    Renderer::getInstance().present();
}

void GameScene::generateRandomEngine()
{
    _gen = []{
            std::random_device rd;
            std::array<std::uint32_t, 8> seed_data;
            auto now = static_cast<unsigned>(
                std::chrono::steady_clock::now().time_since_epoch().count());
            for (std::size_t i = 0; i < seed_data.size(); ++i)
            {
                seed_data[i] = rd() ^ now ^ static_cast<unsigned>(i);
            }
            std::seed_seq seq(seed_data.begin(), seed_data.end());
            return std::mt19937(seq);
        }();
}

void GameScene::savePieceOnBoard() const
{
    _gameField.board->savePieceOnBoard(*_currentPieceData);

}

bool GameScene::isValidRotation() const
{
    const auto coords = _currentPieceData->piece->getBlocksCoord();
    for (const auto& block : coords)
    {
        int x = _currentPieceData->position.x + block.x - _currentPieceData->piece->getCurrentDeltaOrigin().x;
        int y = _currentPieceData->position.y + block.y - _currentPieceData->piece->getCurrentDeltaOrigin().y;

        // Check board bounds
        if (x < 0 || x >= BoardConsts::s_columns ||
            y < 0 || y >= BoardConsts::s_rows)
        {
            return false;
        }

        // Check collisions with existing blocks on board
        if (_gameField.willHitBlockOnBoard(_currentPieceData))
        {
            return false;
        }
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

void GameScene::gameplayStateLogic(const float deltaTime)
{
    const GameRuleset& currentRuleset = _gameField.getGameRulesetData();
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

        _currentPieceData->position = BoardConsts::s_spawnGridPosition;
        applyOriginDeltaToPosition(_currentPieceData, true);

        _currentPieceTimeToDrop = 0.0f;
        _gameState = GameState::Falling;
    }
    else
    {
        _currentPieceTimeToDrop += deltaTime;
    }

    if (_gameState == GameState::SoftDrop)
    {
        if (_currentPieceTimeToDrop >= currentRuleset.softDropTime)
        {
            if (_currentPieceData->position.y < BoardConsts::s_rows - _currentPieceData->piece->getPieceArea().y)
            {
                _currentPieceData->position.y++;
                _softDropAccumulation += _gameField.getGameRulesetData().softDropPointsPerLine;
                _currentPieceTimeToDrop = 0.0f;
                if (_gameField.willHitBlockOnBoard(_currentPieceData))
                {
                    _currentPieceData->position.y--;
                    _gameState = GameState::Spawning;
                }
            }
            else
            {
                _gameState = GameState::Spawning;
            }

            if (_gameState == GameState::Spawning)
            {
                _score += _softDropAccumulation;
                _lockedSoftDrop = true;
            }
        }

    }
    else if (_gameState == GameState::Falling) {
        if (_currentPieceTimeToDrop >= _gameField.currentSpeed)
        {
            if (_currentPieceData->position.y < BoardConsts::s_rows - _currentPieceData->piece->getPieceArea().y)
            {
#if DEBUG_BUILD
                if (!_freezeFall) {
#endif
                    _currentPieceData->position.y++;
                    _softDropAccumulation = _gameField.getGameRulesetData().softDropPointsPerLine;
                    if (_gameField.willHitBlockOnBoard(_currentPieceData))
                    {
                        _currentPieceData->position.y--;
                        if (!_gameField.getGameRulesetData().softDropPointsCountOnlyIfHits)
                        {
                            _score += _softDropAccumulation;
                        }
                        _gameState = GameState::Spawning;
                    }
#if DEBUG_BUILD
                }
#endif
                _currentPieceTimeToDrop = 0.0f;
            }
            else
            {
                _gameState = GameState::Spawning;
            }
        }
    }

    if (_gameState == GameState::Spawning)
    {
        _softDropAccumulation = 0;
        savePieceOnBoard();
    }
}

void GameScene::updateScore()
{

}

void GameScene::cleanup()
{
}
