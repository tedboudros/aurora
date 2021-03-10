#include <SDL2/SDL.h>
#include <SDL2/SDL_image.h>

#include "Entity.hpp"
#include "Math.hpp"
#include "Animation/Easing.h"

Entity::Entity(Vector4f p_dest, SDL_Texture* p_texture) :dest(p_dest), texture(p_texture) {
	currentFrame.x = 0;
	currentFrame.y = 0;
	currentFrame.w = 1024;
	currentFrame.h = 1024;
}; 

void Entity::animate(float p_deltaTime) {

	if(isAnimationActive == true){
		currentAnimationTime += p_deltaTime;

		if(currentAnimationTime >= animationTime) {
			this->finishAnimation();
		}else {
			const float animationPercentageDone = CubicEaseInOut(currentAnimationTime / animationTime);

			const float animationDiffX = animationRect.x - originalRect.x;
			const float newX = originalRect.x + (animationDiffX * animationPercentageDone);

			const float animationDiffY = animationRect.y - originalRect.y;
			const float newY = originalRect.y + (animationDiffY * animationPercentageDone);

			const float animationDiffW = animationRect.w - originalRect.w;
			const float newW = originalRect.w + (animationDiffW * animationPercentageDone);

			const float animationDiffH = animationRect.h - originalRect.h;
			const float newH = originalRect.h + (animationDiffH * animationPercentageDone);

			dest.x = newX;
			dest.y = newY;
			dest.w = newW;
			dest.h = newH;
		}
	}
};