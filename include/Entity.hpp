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

	void setX(float p_x) {
		dest.x = p_x;
	};

	void setY(float p_y) {
		dest.y = p_y;
	};

	void setW(float p_w) {
		dest.w = p_w;
	};

	void setH(float p_h) {
		dest.h = p_h;
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