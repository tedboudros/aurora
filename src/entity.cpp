#include <SDL2/SDL.h>
#include <SDL2/SDL_image.h>

#include "Entity.hpp"
#include "Math.hpp"

Entity::Entity(Vector4f p_dest, SDL_Texture* p_texture) :dest(p_dest), texture(p_texture) {
	currentFrame.x = 0;
	currentFrame.y = 0;
	currentFrame.w = 1024;
	currentFrame.h = 1024;
};