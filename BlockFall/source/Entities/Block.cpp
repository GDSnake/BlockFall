#include "Block.h"

#include <stdexcept>
#include <SDL3/SDL3_image/SDL_image.h>

Block::Block(const std::string& filePath) : _sprite(nullptr, SDL_DestroyTexture)
{
    SDL_Surface* tempSurface = IMG_Load(filePath.c_str());
    auto texture = SDL_CreateTextureFromSurface(nullptr, tempSurface);
    if (!texture)
    {
        throw std::runtime_error(std::string("Error loading the texture %s", SDL_GetError()));
    }
    _sprite.reset(texture);
}

Block::~Block()
{
}
