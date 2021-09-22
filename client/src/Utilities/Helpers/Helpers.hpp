#pragma once

#include <SDL2/SDL.h>
#include <SDL2/SDL_image.h>
#include <iostream>
#include <string>
#include <stdio.h>
#include <time.h>

namespace utils {
	inline Uint64 hireTime() {
		return SDL_GetPerformanceCounter();
	}

    const inline std::string currentDateTime(std::string format = "%Y-%m-%d.%X") {
        time_t     now = time(0);
        struct tm  tstruct;
        char       buf[80];
        tstruct = *localtime(&now);
        // Visit http://en.cppreference.com/w/cpp/chrono/c/strftime
        // for more information about date/time format
        strftime(buf, sizeof(buf), format.c_str(), &tstruct);

        return std::string(buf);
    }
}