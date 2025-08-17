#include "Block.h"
#include "Renderer.h"

#include <stdexcept>
#include <SDL3_image/SDL_image.h>

Block::Block(const std::string& filePath) : _sprite(nullptr, SDL_DestroyTexture)
{
    _sourceRect = {0,0,192,192};
    SDL_Surface* tempSurface = IMG_Load(filePath.c_str());
    auto texture = SDL_CreateTextureFromSurface(Renderer::getInstance().getRenderer(), tempSurface);
    SDL_DestroySurface(tempSurface);
    if (!texture)
    {
        throw std::runtime_error(std::string("Error loading the texture %s", SDL_GetError()));
    }
    _sprite = std::shared_ptr<SDL_Texture>(texture, SDL_DestroyTexture);
}

std::shared_ptr<SDL_Texture> Block::getSprite() const
{
    return _sprite;
}

const SDL_FRect& Block::getSourceRect() const
{
    return _sourceRect;
}
