#include <SDL2/SDL.h>
#include <SDL2/SDL_image.h>

#include "Entity.hpp"

Entity::Entity(float p_x, float p_y, SDL_Texture* p_texture) :x(p_x), y(p_y), texture(p_texture) {
	currentFrame.x = 0;
	currentFrame.y = 0;
	currentFrame.w = 1000;
	currentFrame.h = 1000;
};

float Entity::getX() {
	return x;
}

float Entity::getY() {
	return y;
}

SDL_Texture* Entity::getTexture() {
	return texture;
}

SDL_Rect Entity::getCurrentFrame() {
	return currentFrame;
}