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

void Renderer::drawPiece(std::array<std::shared_ptr<Block>, PieceConsts::maxPieceArea> pieceBlocks, float squareSize, float paddingBetweenBlocks, SDL_FPoint origin)
{
    for (int i = 0; i < pieceBlocks.max_size(); ++i)
    {
        if (!pieceBlocks[i])
        {
            continue;
        }
        float xx = ((i%4) * squareSize) + origin.x + paddingBetweenBlocks;
        float yy = ((i/4) * squareSize) + +origin.y + paddingBetweenBlocks;
        SDL_FRect destinationRectange {xx, yy ,squareSize, squareSize};
        

        drawBlock(pieceBlocks[i], destinationRectange);
    }
}

void Renderer::drawBlock(std::shared_ptr<Block> block, const SDL_FRect& destinationRectange)
{
    SDL_SetRenderDrawColor(_renderer, 255, 255, 255, 255);

    // Test Code
    if (!block) {
        std::cout << "Error: Unable to load image\n";
    }
    
    blitSurface(block->getSprite().get(), &block->getSourceRect(), &destinationRectange);
    // End Test
}

void Renderer::drawBoard()
{


    static SDL_FRect lineArray[30];

    for (int i = 0; i < 30; ++i)
    {
        float thickness = 5;
        SDL_FRect rect;
        if (i < 10)
        {
            rect = { (i * 25.0f)+50, 50, thickness, 480 };
        }
        else
        {
            rect = { 50.0f, ((i-10) * 25.0f) + 50, 225, thickness };
        }
        lineArray[i] = rect;
    }

    SDL_SetRenderDrawColor(_renderer, 0, 0, 0, SDL_ALPHA_OPAQUE);
    SDL_RenderFillRects(_renderer, lineArray, 30);
    SDL_SetRenderDrawColor(_renderer, 255, 255, 255, 255);

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
