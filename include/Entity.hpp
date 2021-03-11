#pragma once
#include <SDL2/SDL.h>
#include <SDL2/SDL_image.h>

#include "Math.hpp"
#include "Animation/Easing.h"

class Entity {

public: 
	Entity(Vector4f p_dest, SDL_Texture* p_texture);

	Vector4f& getDest() {
		return dest;
	};

	void setAnimation(Vector4f p_animationRect, float p_animationTime) {
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
	Vector4f dest;
	SDL_Rect originalDimensions;
	SDL_Texture* texture;
	float animationTime;
	float currentAnimationTime;
	bool isAnimationActive = false;
	Vector4f animationRect;
	Vector4f originalRect;
};