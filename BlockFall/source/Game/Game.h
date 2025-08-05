#include <memory>
#include <SDL3/SDL.h>

class Scene;

class Game
{
public:
	Game();
	~Game();

	bool init();
	void run();
	void shutdown();

private:
	void handleEvents();
	void update(float dt);
	void render();

	SDL_Window* _window = nullptr;
	SDL_Renderer* _renderer = nullptr;
	bool _isRunning = false;

	//std::unique_ptr<Scene> _currentScene;

	Uint64 _lastTime = 0;
	float _deltaTime = 0.0f;
};