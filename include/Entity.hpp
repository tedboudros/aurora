#pragma once
#include <SDL2/SDL.h>
#include <SDL2/SDL_image.h>

#include "Math.hpp"

class Entity {

public: 
	Entity(Vector4f p_dest, SDL_Texture* p_texture);

	Vector4f& getDest() {
		return dest;
	};

	SDL_Texture* getTexture() {
		return texture;
	};

	SDL_Rect getCurrentFrame() {
		return currentFrame;
	};

private:
	Vector4f dest;
	SDL_Rect currentFrame;
	SDL_Texture* texture;
};