#pragma once
#include <memory>
#include <SDL3/SDL_render.h>

#include "Piece.h"

struct Board;
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

	void drawPiece(const std::array<SDL_Point, PieceConsts::numBlocks>& pieceBlocksCoord, std::shared_ptr<Piece> piece, float squareSize, float paddingBetweenBlocks, SDL_FPoint origin = SDL_FPoint(0.0f, 0.0f));
	void drawBlock(const std::shared_ptr<Block> block, const SDL_FRect& destinationRectangle);
	void drawBoard(const Board& board);
	void drawPreviewWindow(const std::array<SDL_Point, PieceConsts::numBlocks>& pieceBlocksCoord, std::shared_ptr<Piece> piece, float windowSize, float paddingBetweenBlocks, SDL_FPoint origin = SDL_FPoint(0.0f, 0.0f));
	void present();
	
	/* void clear
	* Clears the screen
	*/
	void clear();

	/* SDL_Renderer* getRenderer
	* Returns the renderer
	*/
	SDL_Renderer* getRenderer() const;

	void shutdown();


private:
	Renderer();
	~Renderer();

	/*void blitSurface
    * Draws a texture to a certain part of the screen
    */
	void blitSurface(SDL_Texture* source, const SDL_FRect* sourceRectangle, const SDL_FRect* destinationRectangle);

	SDL_Window* _window = nullptr;
	SDL_Renderer* _renderer = nullptr;

};