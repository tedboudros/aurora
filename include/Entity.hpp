#pragma once
#include <SDL2/SDL.h>
#include <SDL2/SDL_image.h>

#include "Math.hpp"
#include "Animation/Easing.h"

class Entity {

public: 
	Entity(MultiSize p_dest, SDL_Texture* p_texture);

	MultiSize& getDest() {
		return dest;
	};

	void setAnimation(MultiSize p_animationRect, float p_animationTime) {
		animationTime = p_animationTime;
		currentAnimationTime = 0;

		originalRect = dest;
		animationRect = p_animationRect;

		isAnimationActive = true;

	}

	void finishAnimation() {
		originalRect = animationRect;
		dest = animationRect;

		isAnimationActive = false;
	}

	void animate(float p_deltaTime);

	SDL_Texture* getTexture() {
		return texture;
	};

	SDL_Rect getOriginalDimensions() {
		return originalDimensions;
	};

private:
	MultiSize dest;
	SDL_Rect originalDimensions;
	SDL_Texture* texture;
	float animationTime;
	float currentAnimationTime;
	bool isAnimationActive = false;
	MultiSize animationRect;
	MultiSize originalRect;
};