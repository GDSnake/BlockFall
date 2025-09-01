#pragma once
#include "Scene.h"

#include <memory>
#include <random>

#include "Config/Config.h"
#include "Entities/EntityStructures.h"
#include "Entities/Piece.h"
#include "Game/GameField.h"
#include "Input/InputManager.h"
struct TTF_Font;
enum class GameState : uint8_t
{
    Starting = 0,
    Falling,
    SoftDrop,
    Spawning,
    Clearing,
    GameOver
};

class GameScene : public Scene
{
public:
    GameScene(GameRulesetsEnum ruleset);
    ~GameScene() override;


    void init() override;
    void handleInput() override;
    void update(const float deltaTime) override;
    void cleanup() override;

    inline std::string getName() const override { return "GameScene"; }

    void handleEvents(const SDL_Event& event) const;
    bool shouldQuit() const;

private:
    void render() override;

    void generateRandomEngine();

    void handlePieceHitting();
    bool isValidRotation() const;
    bool canSoftDrop() const;
    bool canSelectNewPiece() const;
    void gameplayStateLogic(const float deltaTime);

    void updateScore();

    GameState _gameState = GameState::Starting;
    GameField _gameField;
    std::shared_ptr<PieceData> _currentPieceData;
    std::shared_ptr<Piece> _previewNextPiece;
    std::unique_ptr<InputManager> _input;
    const ConfigData& _configData = Config::getInstance().getConfigData();
    float _currentPieceTimeToDrop = 0;
    bool _lockedSoftDrop = false;
    bool _movingHorizontally = false;
    std::mt19937 _gen;

    //Score
    uint64_t _score = 0;
    TTF_Font* _scoreFont = nullptr;
    std::string _scoreText;
    uint64_t _softDropAccumulation = 0;
#if DEBUG_BUILD
    bool _freezeFall = false;
#endif
};
