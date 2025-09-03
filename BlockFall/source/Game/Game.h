#pragma once
#include <memory>

#include <SDL3/SDL_stdinc.h>
#include "Scenes/GameScene.h"

class Game
{
public:
	Game() = default;
	~Game();

	bool init();
	void run();
    static void shutdown();

private:
	void handleEvents();
	void update(float dt);

	bool _isRunning = false;

	//std::unique_ptr<Scene> _currentScene;

	Uint64 _lastTime = 0;
	float _deltaTime = 0.0f;
    Uint32 _delayTimer = 0;
	std::unique_ptr<GameScene> _scene;
};
