#include "SpriteManager.h"

#include <SDL3_image/SDL_image.h>

#include "Entities/Block.h"
#include "Config/Config.h"


SDL_Surface* SpriteManagerHelpers::loadImage(const std::string& filePath)
{
    return IMG_Load(filePath.c_str());
}

std::shared_ptr<Block> SpriteManager::getBlock(BlockTypes block)
{
    return _spritePtrArray[static_cast<int>(block)];
}

SpriteManager::SpriteManager()
{
    loadSprites();
}

SpriteManager::~SpriteManager()
{
    shutdown();
}

void SpriteManager::shutdown()
{
    _spritePtrArray.fill(nullptr);
}

void SpriteManager::loadSprites()
{
    const std::string blocksSpritePath = Config::getInstance().getConfigData().blocksFolder;
    for (std::uint8_t i = 0; i < static_cast<std::uint8_t>(BlockTypes::Total); ++i)
    {
       _spritePtrArray[i] = std::make_shared<Block>(std::format("{}/Block{}.png", blocksSpritePath, BlockType::to_string(static_cast<BlockTypes>(i))));
    }
}