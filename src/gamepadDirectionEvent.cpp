
#include <SDL2/SDL.h>
#include <SDL2/SDL_image.h>
#include <iostream>

#include "Gamepad/GamepadDirectionEvent.hpp"

GamepadDirectionEvent::GamepadDirectionEvent() {
	SDL_JoystickEventState(SDL_ENABLE);

	 for(int i=0; i < SDL_NumJoysticks(); i++ ) {
	 	{
	 		SDL_Joystick* joystick;
	 		joystick = SDL_JoystickOpen(i);
        	std::cout << "Joystick " << i+1 << " connected: " << SDL_JoystickName(joystick) << std::endl;
	 	}
    }
};