#pragma once
#include <memory>

#include <SDL3/SDL_stdinc.h>
#include "Scenes/GameScene.h"
#include "AssetManagers/AudioManager.h"

class Game
{
public:
	Game() = default;
	~Game();

	bool init();
	void run();
	void shutdown();

private:
	void handleEvents();
	void update(float dt);

	bool _isRunning = false;

	//std::unique_ptr<Scene> _currentScene;
	std::unique_ptr<AudioManager> _audioManager;
	//std::unique_ptr<SpriteManager> _spriteManager;

	Uint64 _lastTime = 0;
	float _deltaTime = 0.0f;
	std::unique_ptr<GameScene> _scene;
};
