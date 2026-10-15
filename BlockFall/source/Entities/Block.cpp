#include "Block.h"

#include <format>
#include <memory>
#include <stdexcept>
#include <SDL3_image/SDL_image.h>

#include "Core/Renderer.h"

Block::Block(const std::string& filePath) : _sprite(nullptr, SDL_DestroyTexture)
{
    _sourceRect = {0,0,192,192};
    std::unique_ptr<SDL_Surface, decltype(&SDL_DestroySurface)> surface(IMG_Load(filePath.c_str()), SDL_DestroySurface);
    if (!surface)
    {
        throw std::runtime_error(std::format("Failed to load texture '{}': {}", filePath, SDL_GetError()));
    }

    auto texture = SDL_CreateTextureFromSurface(Renderer::getInstance().getRenderer(), surface.get());
    if (!texture)
    {
        throw std::runtime_error(std::format("Failed to create texture from '{}': {}", filePath, SDL_GetError()));
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
