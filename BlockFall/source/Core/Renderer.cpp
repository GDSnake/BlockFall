#include "Renderer.h"

#include <SDL3/SDL_render.h>
#include <SDL3_image/SDL_image.h>
#include <SDL3_ttf/SDL_ttf.h>

#include "AssetManagers/SpriteManager.h"
#include "Config/Config.h"
#include "Entities/Block.h"
#include "Entities/Piece.h"
#include "Game/Board.h"

static std::array<SDL_FRect, (BoardConsts::s_columns + 1) + (BoardConsts::s_rows + 1)> s_gridLinesArray; // +1 for the border lines
static std::array<SDL_FRect, 4> s_gridlessBoardLineArray; // +1 for the border lines
static bool s_initializedLineArray = false; // Initialize only once the s_gridLinesArray

Renderer::Renderer()
{
    Uint32 flags = Config::getInstance().getConfigData().fullscreen ? SDL_WINDOW_FULLSCREEN : 0;

    SDL_CreateWindowAndRenderer(Config::getInstance().getConfigData().title.c_str(),
                                Config::getInstance().getConfigData().width, 
                                Config::getInstance().getConfigData().height, 
                                flags, 
                                &_window, 
                                &_renderer);

    if (SDL_Surface* surface = SpriteManagerHelpers::loadImage(Config::getInstance().getConfigData().logoPath)) {
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

void Renderer::drawBoardContents(const Board& board)
{
    if (board.isEmpty())
    {
        return;
    }
    const auto& boardBlocks = board.getBoardContent();
    for (uint64_t i = 0; i < boardBlocks.size(); ++i)
    {
        if (!boardBlocks[i]) {
            continue; // Skip empty blocks
        }
        const float xx = ((static_cast<float>((i) % BoardConsts::s_columns)) * board.getCellSize()) + board.getBoardOrigin().x;
        const float yy = ((static_cast<float>(i / BoardConsts::s_columns)) * board.getCellSize()) + board.getBoardOrigin().y;

        SDL_FRect destinationRectangle{ xx, yy ,board.getCellSize(), board.getCellSize() };
        drawBlock(*boardBlocks[i], destinationRectangle);
    }
}

void Renderer::drawPiece(std::span<SDL_Point> pieceBlocksCoord, const Piece& piece, float squareSize, SDL_FPoint origin/* = {0.0f, 0.0f}*/, float hiddenRowsYPosition/* = 1.0f*/)
{
    for (const auto& coord : pieceBlocksCoord)
    {
        float xx = (static_cast<float>(coord.x) * squareSize) + origin.x + BoardConsts::s_lineThickness;
        float yy = (static_cast<float>(coord.y) * squareSize) + origin.y + BoardConsts::s_lineThickness;
        if (yy < hiddenRowsYPosition - BoardConsts::s_lineThickness)
        {
            continue;
        }
        SDL_FRect destinationRectangle {xx, yy ,squareSize, squareSize};

        drawBlock(*piece.getBlock(), destinationRectangle);
    }
}

void Renderer::drawBlock(const Block& block, const SDL_FRect& destinationRectangle)
{
    blitSurface(block.getSprite().get(), &block.getSourceRect(), &destinationRectangle);
}

void Renderer::drawBoard(const Board& board, bool drawGrid)
{
    uint8_t columnsPlusBorder = BoardConsts::s_columns + 1;
    uint8_t rowsPlusBorder = BoardConsts::s_rows + 1;
    uint8_t totalLines = columnsPlusBorder + rowsPlusBorder;
    SDL_FPoint origin = board.getBoardOrigin();
    if (!s_initializedLineArray) {
        float cellSize = board.getCellSize();
        int j = 0;
        for (uint8_t i = 0; i < totalLines; ++i)
        {
            SDL_FRect rect;
            if (i < BoardConsts::s_columns + 1) // Column lines
            {
                rect = {
                    .x = (static_cast<float>(i) * cellSize) + origin.x,
                    .y = board.getOriginYPositionWithHiddenRows(),
                    .w = BoardConsts::s_lineThickness,
                    .h = board.getBoardHeight()
                };
            }
            else if (i < BoardConsts::s_columns + 1 + BoardConsts::s_hiddenRows)
            {
                continue;
            }
            else // Row lines
            {
                rect = {
                    .x = origin.x,
                    .y = ((static_cast<float>(i) - static_cast<float>(columnsPlusBorder)) * cellSize) + origin.y,
                    .w = board.getBoardWidth(),
                    .h = BoardConsts::s_lineThickness
                };
            }

            if (i == 0 || i == BoardConsts::s_columns || i == BoardConsts::s_columns + 1 + BoardConsts::s_hiddenRows || i == totalLines - 1) {
                s_gridlessBoardLineArray[j] = rect;
                ++j;
            }
            s_gridLinesArray[i] = rect;
        }
        s_initializedLineArray = true;
    }
    drawBoardContents(board);

    if (drawGrid)
    {
        SDL_SetRenderDrawColor(_renderer, 0, 0, 0, SDL_ALPHA_OPAQUE);
        SDL_RenderFillRects(_renderer, s_gridLinesArray.data(), totalLines);
    }
    else
    {
        SDL_SetRenderDrawColor(_renderer, 0, 0, 0, SDL_ALPHA_OPAQUE);
        SDL_RenderFillRects(_renderer, s_gridlessBoardLineArray.data(), 4);
    }

    SDL_SetRenderDrawColor(_renderer, 100, 82, 86, SDL_ALPHA_OPAQUE);

}

void Renderer::drawPreviewWindow(std::span<SDL_Point> pieceBlocksCoord, const Piece& piece, float windowSize, SDL_FPoint origin/* = {0.0f, 0.0f}*/)
{
    SDL_SetRenderDrawColor(_renderer, 0, 0, 0, SDL_ALPHA_OPAQUE);
    SDL_FRect rect = {
        .x =origin.x,
        .y = origin.y,
        .w = windowSize,
        .h =windowSize
        };
    SDL_RenderRect(_renderer, &rect);
    drawPiece(pieceBlocksCoord, piece, windowSize * 0.2f, SDL_FPoint(origin.x + windowSize * 0.1f, origin.y + windowSize * 0.4f));
}

void Renderer::drawText(TTF_Font* font, const std::string& text, const FontData& fontData, bool updateTexture/* = false*/)
{
    if (updateTexture || !_textTextureMap.contains(fontData.baseText))
    {
        SDL_Surface* textSurface = TTF_RenderText_Blended_Wrapped(font, text.c_str(), text.size(), fontData.color, 0);
        if (!textSurface) return;

        SDL_Texture* rawTexture = SDL_CreateTextureFromSurface(_renderer, textSurface);
        SDL_DestroySurface(textSurface);

        if (!rawTexture) return;

        TexturePtr texture(rawTexture, SDL_DestroyTexture);
        _textTextureMap.insert_or_assign(fontData.baseText, std::move(texture));
    }

    SDL_Texture* textTexture = _textTextureMap.at(fontData.baseText).get();

    float w, h;
    SDL_GetTextureSize(textTexture, &w, &h);

    SDL_FRect textureRectangle = { fontData.origin.x, fontData.origin.y,
                                   w, h };

    SDL_RenderTexture(_renderer, textTexture, nullptr, &textureRectangle);
}

void Renderer::drawTransparentOverlay() const
{
    SDL_SetRenderDrawBlendMode(_renderer, SDL_BLENDMODE_BLEND);
    SDL_SetRenderDrawColor(_renderer, 120, 120, 120, 128);
    SDL_FRect rect = { 0.0f, 0.0f, static_cast<float>(Config::getInstance().getConfigData().width), static_cast<float>(Config::getInstance().getConfigData().height) };
    SDL_RenderFillRect(_renderer, &rect);
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

#if DEBUG_BUILD
void Renderer::writeFPSOnWindowTitle(Uint64 fps)
{
    std::string windowTitle = std::format("{} - FPS: {}", Config::getInstance().getConfigData().title, fps);
    SDL_SetWindowTitle(_window, windowTitle.c_str());
}
#endif