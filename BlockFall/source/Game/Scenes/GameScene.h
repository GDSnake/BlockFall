#pragma once
#include <memory>
#include <random>

#include "Scene.h"
#include "GameField.h"
#include "InputManager.h"

#include "EntityStructures.h"

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
    GameScene() = default;
    ~GameScene() override;


    void init() override;
    void handleInput(const float dt) override;
    void update(const float deltaTime) override;
    void cleanup() override;

    inline std::string getName() const override { return "GameScene"; }

    void handleEvents(const SDL_Event& event) const;
    bool shouldQuit() const;

private:
    void render() override;

    void savePieceOnBoard() const;
    bool isValidRotation(Piece& piece, const SDL_Point& position) const;
    bool canSoftDrop() const;
    bool canSelectNewPiece() const;

    GameState _gameState;
    GameField _gameField;
    std::shared_ptr<PieceData> _currentPieceData;
    std::shared_ptr<Piece> _previewNextPiece;
    std::unique_ptr<InputManager> _input;
    const ConfigData& _configData = Config::getInstance().getConfigData();
    int currentLevel = 0;
    float _currentPieceTimeToDrop;
    bool _lockedSoftDrop = false;
    bool _movingHorizontally = false;
    std::random_device _rd;
    std::mt19937 _gen;
};
