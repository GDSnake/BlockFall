#include "Game.h"

#include <SDL3/SDL_init.h>
#include <SDL3/SDL_log.h>
#include <SDL3/SDL_timer.h>

#include "SpriteManager.h"
#include "Renderer.h"
#include "Config.h"

Game::~Game()
{
    shutdown();
}

bool Game::init()
{
    // Initialize SDL (video + events)
    if (!SDL_Init(SDL_INIT_EVENTS | SDL_INIT_VIDEO | SDL_INIT_AUDIO )) {
        SDL_Log("SDL Init Error: %s \n",SDL_GetError());
        return false;
    }
    _scene = std::make_unique<GameScene>();
    _isRunning = true;
    

    // Initialize AudioManager
    _audioManager = std::make_unique<AudioManager>(Config::getInstance().getConfigData().bgmPath); // Maybe also be a Singleton

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
        _scene->render();
    }
}

void Game::shutdown()
{
    // Reset Scene
    Renderer::getInstance().shutdown();

    SDL_Quit();
}

void Game::handleEvents()
{
    SDL_Event event;
    while (SDL_PollEvent(&event)) {
        if (event.type == SDL_EVENT_QUIT) {
            _isRunning = false;
        }
        //if (_scene)
            //_scene->handleEvents(event);
    }
}

void Game::update(float dt)
{
}

