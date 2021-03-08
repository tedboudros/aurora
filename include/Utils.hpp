#pragma once

#include <SDL2/SDL.h>
#include <SDL2/SDL_image.h>

namespace utils {
	inline Uint64 hireTime() {
		return SDL_GetPerformanceCounter();
	}
}