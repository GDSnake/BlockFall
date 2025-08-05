#include "Game.h"
#include "../Config/Config.h"

Game::Game()
{
}

Game::~Game()
{
}

bool Game::init()
{
    // Initialize SDL (video + events)
    if (!SDL_Init(SDL_INIT_EVENTS | SDL_INIT_VIDEO | SDL_INIT_AUDIO )) {
        SDL_Log("SDL Init Error: %s \n",SDL_GetError());
        return false;
    }
    Uint32 flags = Config::getInstance().getConfigData().fullscreen ? SDL_WINDOW_FULLSCREEN : 0;

	_window = SDL_CreateWindow(Config::getInstance().getConfigData().title.c_str(),
                              Config::getInstance().getConfigData().width, Config::getInstance().getConfigData().height,
                              flags);

    if (!_window) {
        SDL_Log("Window Error: %s \n",SDL_GetError());
        SDL_Quit();
        return false;
    }

    _renderer = SDL_CreateRenderer(_window, NULL);
    if (!_renderer) {
        SDL_Log("Failed to create renderer: %s", SDL_GetError());
        return false;
    }
    _isRunning = true;

    //_currentScene = std::make_unique<GameScene>(_renderer);

    _lastTime = SDL_GetTicks();
    return true;
}

void Game::run()
{
    while (_isRunning)
    {
        Uint64 current = SDL_GetTicks();
        _deltaTime = static_cast<float>(current - _lastTime) / 1000.0f;
        _lastTime = current;

        handleEvents();
        update(_deltaTime);
        render();
    }
}

void Game::shutdown()
{
    // Reset Scene

    if (_renderer) {
        SDL_DestroyRenderer(_renderer);
        _renderer = nullptr;
    }

    if (_window) {
        SDL_DestroyWindow(_window);
        _window = nullptr;
    }

    SDL_Quit();
}

void Game::handleEvents()
{
    SDL_Event event;
    while (SDL_PollEvent(&event)) {
        if (event.type == SDL_EVENT_QUIT) {
            _isRunning = false;
        }
        //if (_currentScene)
            //_currentScene->handleEvent(event);
    }
}

void Game::update(float dt)
{
}

void Game::render()
{
    SDL_SetRenderDrawColor(_renderer, 0, 0, 0, 255);
    SDL_RenderClear(_renderer);

    //if (_currentScene)
        //_currentScene->render();

    SDL_RenderPresent(_renderer);
}
