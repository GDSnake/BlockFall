#include "Renderer.h"

#include <SDL3/SDL_render.h>
#include <SDL3/SDL3_image/SDL_image.h>
#include "../Config/Config.h"

Renderer::Renderer()
{
    Uint32 flags = Config::getInstance().getConfigData().fullscreen ? SDL_WINDOW_FULLSCREEN : 0;

    SDL_CreateWindowAndRenderer(Config::getInstance().getConfigData().title.c_str(),
                                Config::getInstance().getConfigData().width, 
                                Config::getInstance().getConfigData().height, 
                                flags, 
                                &_window, 
                                &_renderer);

    if (SDL_Surface* surface = IMG_Load(Config::getInstance().getConfigData().logoPath.c_str())) {
        SDL_SetWindowIcon(this->_window, surface);
        SDL_DestroySurface(surface);
    }


    if (!_window) {
        SDL_Log("Window Error: %s \n", SDL_GetError());
        SDL_Quit();
        return;
    }

    if (!_renderer) {
        SDL_Log("Failed to create renderer: %s", SDL_GetError());
        return;
    }
}

Renderer::~Renderer()
{
}

SDL_Surface* Renderer::loadImage(const std::string& filePath)
{
    return IMG_Load(filePath.c_str());

}

void Renderer::blitSurface(SDL_Texture* texture, const SDL_FRect* sourceRectangle, const SDL_FRect* destinationRectangle)
{
    SDL_RenderTexture(_renderer, texture, sourceRectangle, destinationRectangle);

}

void Renderer::draw()
{
    SDL_SetRenderDrawColor(_renderer, 0, 0, 0, 255);
    SDL_RenderClear(_renderer);

    //if (_currentScene)
        //_currentScene->render();
    // Test Code
    SDL_Surface* tempSurface = loadImage(std::string(Config::getInstance().getConfigData().blocksFolder + "/BlockYellow.png"));
    SDL_Texture* sprite = SDL_CreateTextureFromSurface(getRenderer(), tempSurface);
    if (!sprite) {
        std::cout << "Error: Unable to load image" << std::endl;
    }
    SDL_DestroySurface(tempSurface);
    SDL_FRect destinationRectange = { 0, 0, 100, 100 };
    SDL_FRect sourceRect = { 0, 0, 192, 192 };
    blitSurface(sprite, &sourceRect, &destinationRectange);
    // End Test
    SDL_RenderPresent(_renderer);
    SDL_DestroyTexture(sprite);
}

void Renderer::clear()
{
    SDL_RenderClear(_renderer);

}

SDL_Renderer* Renderer::getRenderer() const
{
    return _renderer;
}

void Renderer::shutdown()
{
    if (_renderer)
    {
        SDL_DestroyRenderer(_renderer);
        _renderer = nullptr;
    }

    if (_window) {
        SDL_DestroyWindow(_window);
        _window = nullptr;
    }

}
