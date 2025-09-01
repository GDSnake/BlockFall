#include "Game.h"

#include <SDL3/SDL_init.h>
#include <SDL3/SDL_log.h>
#include <SDL3/SDL_timer.h>
#include <SDL3_ttf/SDL_ttf.h>

#include <algorithm>

#include "AssetManagers/FontsManager.h"
#include "Core/Renderer.h"

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
    if (!TTF_Init()) {
        std::cerr << std::format("Error Initializing TTF: {} \n", SDL_GetError());
    }

    FontsManager::getInstance();
    _scene = std::make_unique<GameScene>();
    _scene->init();
    _isRunning = true;
    

    // Initialize AudioManager
    _audioManager = std::make_unique<AudioManager>(Config::getInstance().getConfigData().bgmPath); // Maybe also be a Singleton

    _lastTime = SDL_GetTicks();
    return true;
}

void Game::run()
{
    Uint64 frames = 0;
    Uint64 fpsResetTick = 0;
    const float targetFrameTime = 1.0f / static_cast<float>(Config::getInstance().getConfigData().targetFPS);
    while (_isRunning)
    {
        Uint64 current = SDL_GetTicks();
        _deltaTime = static_cast<float>(current - _lastTime) / 1000.0f;
        _lastTime = current;

        handleEvents();
        update(_deltaTime);
        frames++;

        if (current - fpsResetTick >= 1000) {
            fpsResetTick = current;
#if DEBUG_BUILD
            Renderer::getInstance().writeFPSOnWindowTitle(frames);
#endif
            frames = 0;
        }

        if (_scene->shouldQuit())
        {
            _isRunning = false;
        }

        // Cap frame rate
        Uint64 frameEnd = SDL_GetTicks();
        float frameDuration = static_cast<float>(frameEnd - current) / 1000.0f;

        if (frameDuration < targetFrameTime) {
            Uint32 delayMs = static_cast<Uint32>((targetFrameTime - frameDuration) * 1000.0f);
            SDL_Delay(delayMs);
        }
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
        if (_scene) {
            _scene->handleEvents(event);
        }
    }
}

void Game::update(float dt)
{
    _scene->update(dt);
}

