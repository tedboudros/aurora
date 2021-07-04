#include <SDL2/SDL.h>
#include <SDL2/SDL_image.h>
#include <iostream>

#include "RenderWindow.hpp"
#include "Entity.hpp"
#include "Text.hpp"
#include "Math.hpp"

float calculateSize(Size value, Vector2f windowDimensions) {
	return (value.val / 100)  * (value.type == SIZE_HEIGHT ? windowDimensions.y : windowDimensions.x);
}

RenderWindow::RenderWindow(const char* p_title, int p_width, int p_height) :window(NULL), renderer(NULL), windowDims(Vector2f(p_width, p_height)) {
	window = SDL_CreateWindow(p_title, SDL_WINDOWPOS_UNDEFINED, SDL_WINDOWPOS_UNDEFINED, p_width, p_height, SDL_WINDOW_SHOWN | SDL_WINDOW_VULKAN );//| SDL_WINDOW_FULLSCREEN_DESKTOP );

	if(window == NULL) {
		std::cout << "Window has failed to init. Error: " << SDL_GetError() << std::endl; 
	}

	SDL_SetHint( SDL_HINT_RENDER_SCALE_QUALITY, "1" );

	renderer = SDL_CreateRenderer(window, -1, SDL_RENDERER_ACCELERATED | SDL_RENDERER_PRESENTVSYNC);// );//
	SDL_ShowCursor(false); // Hides the cursor;
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

void RenderWindow::render(Entity& p_entity) {

	SDL_Rect src, dest;

	src.x = p_entity.getOriginalDimensions().x;
	src.y = p_entity.getOriginalDimensions().y;
	src.w = p_entity.getOriginalDimensions().w;
	src.h = p_entity.getOriginalDimensions().h;

	dest.x = calculateSize(p_entity.getDest().x, windowDims);
	dest.y = calculateSize(p_entity.getDest().y, windowDims);
	dest.w = calculateSize(p_entity.getDest().w, windowDims);
	dest.h = calculateSize(p_entity.getDest().h, windowDims);

	SDL_RenderCopy(renderer, p_entity.getTexture(), &src, &dest);
}

void RenderWindow::render(Text& p_text) {
	SDL_Rect src, dest;
	
	p_text.updateTexture(renderer);
	
	src.x = src.y = 0;
	src.w = p_text.getTextureWidth();
	src.h = p_text.getTextureHeight();
	
	dest.x = calculateSize(p_text.getDestRect().x, windowDims);
	dest.y = calculateSize(p_text.getDestRect().y, windowDims);

	// Use the height from the destRect (MultiSize) as a scale for the font.
	if (p_text.getUseTextureSize())
	{
		dest.w = p_text.getTextureWidth() * (p_text.getFontScale());
		dest.h = p_text.getTextureHeight() * (p_text.getFontScale());
	} else {
		dest.w = calculateSize(p_text.getDestRect().w, windowDims);
		dest.h = calculateSize(p_text.getDestRect().h, windowDims);
	}

	SDL_RenderCopy(renderer, p_text.getTexture(), &src, &dest);
}

void RenderWindow::display() {
	SDL_RenderPresent(renderer);
}

int RenderWindow::getRefreshRate() {
	int displayIndex = SDL_GetWindowDisplayIndex(window);

	SDL_DisplayMode mode;

	SDL_GetDisplayMode(displayIndex, 0, &mode);

	return mode.refresh_rate;
}

int RenderWindow::getDisplayWidth() {
	int displayIndex = SDL_GetWindowDisplayIndex(window);

	SDL_DisplayMode mode;

	SDL_GetDisplayMode(displayIndex, 0, &mode);

	return mode.w;
}


int RenderWindow::getDisplayHeight() {
	int displayIndex = SDL_GetWindowDisplayIndex(window);

	SDL_DisplayMode mode;

	SDL_GetDisplayMode(displayIndex, 0, &mode);

	return mode.h;
}

Vector2f RenderWindow::getWindowDimensions() {
	int w,h;
	SDL_GetWindowSize(window, &w, &h);
	return Vector2f(w,h);
}

void RenderWindow::setFullScreen(bool isFullScreen) {
	SDL_SetWindowFullscreen(window, isFullScreen);
}