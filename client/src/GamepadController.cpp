
#include <SDL2/SDL.h>
#include <SDL2/SDL_image.h>
#include <iostream>

#include "Gamepad/GamepadController.hpp"

const int axisThreshold = 20000;
const int spamInterval = 135;
const int spamTimeout = 400;

GamepadController::GamepadController() {
	SDL_JoystickEventState(SDL_ENABLE);

	 for(int i=0; i < SDL_NumJoysticks(); i++ ) {
	 	{
	 		SDL_Joystick* joystick;
	 		joystick = SDL_JoystickOpen(i);
        	std::cout << "Joystick " << i+1 << " connected: " << SDL_JoystickName(joystick) << std::endl;
	 	}
    }
};

void GamepadController::execFrame(SDL_Event& event) {	
    prevIsRightIn = isRightIn;
    prevIsLeftIn = isLeftIn;
 
	if( event.jaxis.axis == 0) {
    	if(lastAxisZeroValue <= axisThreshold && event.jaxis.value > axisThreshold){
    		// Right In
    		isRightIn = true;
            isRightFirstIn = true;
    	}

    	if(lastAxisZeroValue > axisThreshold && event.jaxis.value <= axisThreshold){
    		// Right Out
    		isRightIn = false;
            isRightFirstIn = false;
    	}

    	if(lastAxisZeroValue > -axisThreshold && event.jaxis.value <= -axisThreshold){
    		// Left In
    		isLeftIn = true;
            isLeftFirstIn = true;
    	}

    	if(lastAxisZeroValue <= -axisThreshold && event.jaxis.value > -axisThreshold){
    		// Left Out
    		isLeftIn = false;
            isLeftFirstIn = false;
    	}

    	lastAxisZeroValue = event.jaxis.value;
    }
}

void GamepadController::spamController(double deltaTime) {
    isRightSpamming = false;
    isLeftSpamming = false;

    if(isLeftIn || isRightIn) {
        currentSpamTime += deltaTime; 

        if(currentSpamTime >= ((isRightFirstIn || isLeftFirstIn) ? spamTimeout : spamInterval)) {
            if(isLeftIn) {
                isLeftSpamming = true;

                if(isLeftFirstIn && isLeftIn) {
                    isLeftFirstIn = false;
                }
            }else if(isRightIn) {
                isRightSpamming = true;

                if(isRightFirstIn && isRightIn) {
                    isRightFirstIn = false;
                }
            }

            currentSpamTime = 0;
        }

    }else {
        currentSpamTime = 0;
        isLeftFirstIn = false;
        isRightFirstIn = false;
    }

   

}