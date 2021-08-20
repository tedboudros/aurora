
#include <SDL2/SDL.h>
#include <SDL2/SDL_image.h>
#include <iostream>

#include "Keyboard/KeyboardController.hpp"

const int spamInterval = 135;
const int spamTimeout = 400;

KeyboardController::KeyboardController() {
};

void KeyboardController::execFrame(SDL_Event& event) {	
    prevIsRightIn = isRightIn;s
    prevIsLeftIn = isLeftIn;
 
    if (keyboardKeys[SDL_SCANCODE_RIGHT] && !isRightIn) {
        // Right In
        isRightIn = true;
        isRightFirstIn = true;
    }else if(!keyboardKeys[SDL_SCANCODE_RIGHT] && isRightIn) {
        // Right Out
        isRightIn = false;
        isRightFirstIn = false;
    }

    if (keyboardKeys[SDL_SCANCODE_LEFT] && !isLeftIn) {
        // Left In
        isLeftIn = true;
        isLeftFirstIn = true;
    }else if(!keyboardKeys[SDL_SCANCODE_LEFT] && isLeftIn) {
        // Left Out
        isLeftIn = false;
        isLeftFirstIn = false;
    }

}

void KeyboardController::spamKeyboard(double deltaTime) {
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