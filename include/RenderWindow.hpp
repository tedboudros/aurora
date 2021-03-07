#pragma once
#include <SDL2/SDL.h>
#include <SDL2/SDL_image.h>

class RenderWindow {

public:
	RenderWindow(const char* p_title, int p_width, int p_height);
	void cleanUp();

private:
	SDL_Window* window;
	SDL_Renderer* renderer;

};