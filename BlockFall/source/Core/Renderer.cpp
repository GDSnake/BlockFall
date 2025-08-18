#include "Renderer.h"

#include <SDL3/SDL_render.h>
#include <SDL3_image/SDL_image.h>

#include "Block.h"
#include "Board.h"
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

void Renderer::drawPiece(const std::span<SDL_Point>& pieceBlocksCoord, std::shared_ptr<Piece> piece, float squareSize, float paddingBetweenBlocks, SDL_FPoint origin)
{
    for (const auto& coord : pieceBlocksCoord)
    {
        float xx = (static_cast<float>(coord.x) * squareSize) + origin.x + paddingBetweenBlocks;
        float yy = (static_cast<float>(coord.y) * squareSize) + +origin.y + paddingBetweenBlocks;
        SDL_FRect destinationRectangle {xx, yy ,squareSize, squareSize};
        

        drawBlock(piece->getBlock(), destinationRectangle);
    }
}

void Renderer::drawBlock(std::shared_ptr<Block> block, const SDL_FRect& destinationRectangle)
{
    // Test Code
    if (!block) {
        std::cout << "Error: Unable to load image\n";
    }
    
    blitSurface(block->getSprite().get(), &block->getSourceRect(), &destinationRectangle);
    // End Test
}


void Renderer::drawBoard(const Board& board)
{

    uint8_t columnsPlusBorder = BoardConsts::s_columns + 1;
    uint8_t rowsPlusBorder = BoardConsts::s_rows + 1;
    uint8_t totalLines = columnsPlusBorder + rowsPlusBorder;
    static std::array<SDL_FRect, (BoardConsts::s_columns + 1) + (BoardConsts::s_rows + 1)> lineArray;
    static bool initialized = false;
    SDL_FPoint origin = board.getBoardOrigin();
    if (!initialized) {
        float cellSize = board.getCellSize();
        for (uint8_t i = 0; i < totalLines; ++i)
        {
            SDL_FRect rect;
            if (i < BoardConsts::s_columns + 1)
            {
                rect = {
                    .x = (static_cast<float>(i) * cellSize) + origin.x,
                    .y = origin.y,
                    .w = BoardConsts::s_lineThickness,
                    .h = board.getBoardHeight()
                };
            }
            else
            {
                rect = {
                    .x = origin.x,
                    .y = ((static_cast<float>(i) - static_cast<float>(columnsPlusBorder)) * cellSize) + origin.y,
                    .w = board.getBoardWidth(),
                    .h = BoardConsts::s_lineThickness
                };
            }
            lineArray[i] = rect;
        }
        initialized = true;
    }

    if (Config::getInstance().getConfigData().drawGrid)
    {
        SDL_SetRenderDrawColor(_renderer, 0, 0, 0, SDL_ALPHA_OPAQUE);
        SDL_RenderFillRects(_renderer, lineArray.data(), totalLines);
    }
    //Draw board->arrayBlocks
    SDL_SetRenderDrawColor(_renderer, 100, 82, 86, SDL_ALPHA_OPAQUE);

}

void Renderer::drawPreviewWindow(const std::span<SDL_Point>& pieceBlocksCoord, std::shared_ptr<Piece> piece, float windowSize, float paddingBetweenBlocks, SDL_FPoint origin)
{
    SDL_SetRenderDrawColor(_renderer, 0, 0, 0, SDL_ALPHA_OPAQUE);
    SDL_FRect rect = {
        .x =origin.x,
        .y = origin.y,
        .w = windowSize,
        .h =windowSize
        };
    SDL_RenderRect(_renderer, &rect);
    drawPiece(pieceBlocksCoord, piece, windowSize * 0.2f, paddingBetweenBlocks, SDL_FPoint (origin.x + windowSize * 0.1f, origin.y + windowSize * 0.4f));
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
