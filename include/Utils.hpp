#pragma once

#include <SDL2/SDL.h>
#include <SDL2/SDL_image.h>

namespace utils {
	inline float hireTimeInSeconds() {
		float t = SDL_GetTicks();
		t *= 0.001f;

		return t;
	}
}