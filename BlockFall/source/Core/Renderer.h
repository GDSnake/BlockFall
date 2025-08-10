#pragma once
#include <string>
#include <SDL3/SDL_render.h>

struct SDL_Window;
struct SDL_Renderer;
struct SDL_Surface;
struct SDL_Texture;
struct SDL_Rect;

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

	/*SDL_Surface* loadImage
	* Loads and returns image surface.
	*/
	SDL_Surface* loadImage(const std::string& filePath); // Move to sprite manager

	/*void blitSurface
	* Draws a texture to a certain part of the screen
	*/
	void blitSurface(SDL_Texture* source, const SDL_FRect* sourceRectangle, const SDL_FRect* destinationRectangle);

	/*void draw
	* Renders everything to the screen
	*/
	void draw();

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

	SDL_Window* _window = nullptr;
	SDL_Renderer* _renderer = nullptr;

};