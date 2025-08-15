#pragma once
#include <memory>
#include <string>
#include <SDL3/SDL_render.h>

class Block
{
public:
    Block() = delete;
    Block(const std::string& filePath);

    ~Block() = default;

    std::shared_ptr<SDL_Texture> getSprite() const;
    const SDL_FRect& getSourceRect() const;
private:
    SDL_FRect _sourceRect;
    std::shared_ptr<SDL_Texture> _sprite;
};
