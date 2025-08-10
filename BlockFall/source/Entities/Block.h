#pragma once
#include <memory>
#include <string>
#include <SDL3/SDL_render.h>

class Block
{
public:
    using TexturePtr = std::unique_ptr<SDL_Texture, decltype(&SDL_DestroyTexture)>;
    Block() = delete;
    Block(const std::string& filePath);

    ~Block();
protected:
    SDL_Rect _sourceRect;
    TexturePtr _sprite;
};
