#include "Renderer.h"

#include <SDL3/SDL_render.h>
#include <SDL3/SDL3_image/SDL_image.h>

#include "Block.h"
#include "Config.h"
#include "SpriteManager.h"

Renderer::Renderer()
{
    Uint32 flags = Config::getInstance().getConfigData().fullscreen ? SDL_WINDOW_FULLSCREEN : 0;

    SDL_CreateWindowAndRenderer(Config::getInstance().getConfigData().title.c_str(),
                                Config::getInstance().getConfigData().width, 
                                Config::getInstance().getConfigData().height, 
                                flags, 
                                &_window, 
                                &_renderer);
    if (SDL_Surface* surface = SpriteManagerHelpers::loadImage(ConfigHelpers::getFullPath(Config::getInstance().getConfigData().logoPath))) {
        SDL_SetWindowIcon(_window, surface);
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

void Renderer::blitSurface(SDL_Texture* texture, const SDL_FRect* sourceRectangle, const SDL_FRect* destinationRectangle)
{
    SDL_RenderTexture(_renderer, texture, sourceRectangle, destinationRectangle);

}

void Renderer::drawBlock(std::shared_ptr<Block> block, const SDL_FRect& destinationRectange)
{
    SDL_SetRenderDrawColor(_renderer, 0, 0, 0, 255);

    //if (_currentScene)
        //_currentScene->render();
    // Test Code
    //SDL_Surface* tempSurface = SpriteManagerHelpers::loadImage(std::string(ConfigHelpers::getFullPath(Config::getInstance().getConfigData().blocksFolder) + "/BlockYellow.png"));
    //SDL_Texture* sprite = SDL_CreateTextureFromSurface(getRenderer(), tempSurface);
    if (!block.get()) {
        std::cout << "Error: Unable to load image\n";
    }
    
    blitSurface(block.get()->getSprite().get(), &block.get()->getSourceRect(), &destinationRectange);
    // End Test
    //SDL_DestroyTexture(sprite);
}

void Renderer::present()
{
    SDL_RenderPresent(_renderer);
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
