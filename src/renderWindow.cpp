#include <SDL2/SDL.h>
#include <SDL2/SDL_image.h>
#include <iostream>

#include "RenderWindow.hpp"

RenderWindow::RenderWindow(const char* p_title, int p_width, int p_height) :window(NULL), renderer(NULL) {
	window = SDL_CreateWindow(p_title, SDL_WINDOWPOS_UNDEFINED, SDL_WINDOWPOS_UNDEFINED, p_width, p_height, SDL_WINDOW_SHOWN);

	if(window == NULL) {
		std::cout << "Window has failed to init. Error: " << SDL_GetError() << std::endl; 
	}

	renderer = SDL_CreateRenderer(window, -1, SDL_RENDERER_ACCELERATED);
};

void RenderWindow::cleanUp() {
	SDL_DestroyWindow(window);
}

SDL_Texture* RenderWindow::loadTexture(const char* p_filepath) {
	SDL_Texture* texture = NULL;
	texture = IMG_LoadTexture(renderer, p_filepath);

	if(texture == NULL) {
		std::cout << "Failed to load texture: " << p_filepath << " . Error: " << SDL_GetError() << std::endl;
	}

	return texture;
}

void RenderWindow::clear() {
	SDL_RenderClear(renderer);
}

void RenderWindow::render(SDL_Texture* p_texture) {
	SDL_RenderCopy(renderer, p_texture, NULL, NULL);
}

void RenderWindow::display() {
	SDL_RenderPresent(renderer);
}