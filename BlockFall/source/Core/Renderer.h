#pragma once
#include <span>
#include <SDL3/SDL_render.h>

#include "Entities/Piece.h"
#include "Game/Board.h"
#include "AssetManagers/FontsManager.h"

struct SDL_Window;
struct SDL_Renderer;
struct SDL_Surface;
struct SDL_Texture;
struct SDL_Rect;
struct TTF_Font;
class Scene;
class Block;

/*Renderer Class
*Holds all information dealing with graphics for the game
*/
class Renderer
{
public:
	static Renderer& getInstance()
	{
		static Renderer instance;
		return instance;
	}

	void drawPiece(std::span<SDL_Point> pieceBlocksCoord, const Piece& piece, float squareSize, SDL_FPoint origin = {0.0f, 0.0f}, float hiddenRowsYPosition = -1.0f);
	void drawBlock(const Block& block, const SDL_FRect& destinationRectangle);
	void drawBoard(const Board& board);
	void drawPreviewWindow(std::span<SDL_Point> pieceBlocksCoord, const Piece& piece, float windowSize, SDL_FPoint origin = {0.0f, 0.0f});

	void drawText(TTF_Font* font, const std::string& text, const FontData& fontData);

	void present();
	void clear();

	SDL_Renderer* getRenderer() const;

	void shutdown();
#if DEBUG_BUILD
	void writeFPSOnWindowTitle(Uint64 fps);
#endif
private:
	Renderer();
	~Renderer();

	/*void blitSurface
    * Draws a texture to a certain part of the screen
    */
	void blitSurface(SDL_Texture* source, const SDL_FRect* sourceRectangle, const SDL_FRect* destinationRectangle);
	void drawBoardContents(const Board& board);

	SDL_Window* _window = nullptr;
	SDL_Renderer* _renderer = nullptr;

};