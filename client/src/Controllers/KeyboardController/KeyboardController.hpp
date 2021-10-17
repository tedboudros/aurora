#pragma once

#include <SDL2/SDL.h>
#include <SDL2/SDL_image.h>
#include <iostream>
#include <vector>

#include "Utilities/Math/Math.hpp"
#include "Rendering/Easing/Easing.h"
#include "Controllers/ControllerState/ControllerState.hpp"

class KeyboardController {

public:
    bool onLeft() {
        return (isLeftIn && !prevIsLeftIn);
    };

    bool onRight() {
        return (isRightIn && !prevIsRightIn);
    };

    bool onRightSpam() {
        return isRightSpamming;
    };

    bool onLeftSpam() {
        return isLeftSpamming;
    };

    void handleNavigationEvents(SDL_Event &event);

    void handleUpdateControllerState(SDL_Event &event, ControllerState *p_controllerState);

    void spamKeyboard(double deltaTime);

private:
    bool isLeftIn = false, isRightIn = false, prevIsLeftIn = false, prevIsRightIn = false, isRightSpamming = false, isLeftSpamming = false, isLeftFirstIn = false, isRightFirstIn = false;
    const Uint8 *keyboardKeys = SDL_GetKeyboardState(NULL);
    double currentSpamTime = 0;
};