#pragma once
#include <array>
#include <memory>
#include <string>

class Block;
struct SDL_Surface;

namespace SpriteManagerHelpers
{
    SDL_Surface* loadImage(const std::string& filePath);
}

enum class BlockTypes : uint8_t
{
    Purple = 0,
    Cyan,
    Green,
    Orange,
    Yellow,
    Red,
    Blue,
    Ghost,
    Total
};

namespace BlockType
{
    inline const std::string to_string(BlockTypes type)
    {
        switch (type)
        {
        case BlockTypes::Purple: return "Purple";
        case BlockTypes::Cyan: return "Cyan";
        case BlockTypes::Green: return "Green";
        case BlockTypes::Orange: return "Orange";
        case BlockTypes::Yellow: return "Yellow";
        case BlockTypes::Red: return "Red";
        case BlockTypes::Blue: return "Blue";
        case BlockTypes::Ghost: return "Ghost";
        default: return "unknown";
        }
    }
}


class SpriteManager
{
public:

    static SpriteManager& getInstance()
    {
        static SpriteManager instance;
        return instance;
    }

    std::shared_ptr<Block> getBlock(BlockTypes block);
private:
    SpriteManager();
    ~SpriteManager();

    void loadSprites();

    std::array<std::shared_ptr<Block>, static_cast<size_t>(BlockTypes::Total)> _spritePtrArray;
};
