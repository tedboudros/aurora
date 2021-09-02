#include <SDL2/SDL.h>
#include <SDL2/SDL_image.h>

#include "Entity.hpp"
#include "Math.hpp"
#include "Animation/Easing.h"

Entity::Entity(MultiSize p_dest, SDL_Texture* p_texture) :dest(p_dest), texture(p_texture) {
	int w, h;

	SDL_QueryTexture(texture, NULL, NULL, &w, &h);

	originalDimensions.x = 0;
	originalDimensions.y = 0;
	originalDimensions.w = w;
	originalDimensions.h = h;
}; 

Entity::Entity() {
	originalDimensions.x = 0;
	originalDimensions.y = 0;
	originalDimensions.w = 0;
	originalDimensions.h = 0;
}; 

void Entity::animate(float p_deltaTime) {

	if(isAnimationActive == true){
		currentAnimationTime += p_deltaTime;

		if(currentAnimationTime >= animationTime) {
			this->finishAnimation();
		}else {
			const float animationPercentageDone = CubicEaseInOut(currentAnimationTime / animationTime);

			const float animationDiffX = animationRect.x.val - originalRect.x.val;
			const float newX = originalRect.x.val + (animationDiffX * animationPercentageDone);

			const float animationDiffY = animationRect.y.val - originalRect.y.val;
			const float newY = originalRect.y.val + (animationDiffY * animationPercentageDone);

			const float animationDiffW = animationRect.w.val - originalRect.w.val;
			const float newW = originalRect.w.val + (animationDiffW * animationPercentageDone);

			const float animationDiffH = animationRect.h.val - originalRect.h.val;
			const float newH = originalRect.h.val + (animationDiffH * animationPercentageDone);

			dest.setX(newX);
			dest.setY(newY);
			dest.setW(newW);
			dest.setH(newH);
		}
	}
};