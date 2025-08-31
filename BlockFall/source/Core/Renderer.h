#pragma once
#include <span>
#include <SDL3/SDL_render.h>

#include "Entities/Piece.h"
#include "Game/Board.h"


struct SDL_Window;
struct SDL_Renderer;
struct SDL_Surface;
struct SDL_Texture;
struct SDL_Rect;
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

	void drawPiece(std::span<SDL_Point> pieceBlocksCoord, const Piece& piece, float squareSize, SDL_FPoint origin = SDL_FPoint(0.0f, 0.0f), float hiddenRowsYPosition = -1.0f);
	void drawBlock(const Block& block, const SDL_FRect& destinationRectangle);
	void drawBoard(const Board& board);
	void drawPreviewWindow(std::span<SDL_Point> pieceBlocksCoord, const Piece& piece, float windowSize, SDL_FPoint origin = SDL_FPoint(0.0f, 0.0f));
	void present();
	void clear();

	SDL_Renderer* getRenderer() const;

	void shutdown();


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