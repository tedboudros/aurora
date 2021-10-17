#pragma once

#include <SDL2/SDL.h>
#include <SDL2/SDL_image.h>

#include "Utilities/Math/Math.hpp"
#include "Rendering/Easing/Easing.h"

#include "Controllers/ControllerState/ControllerState.hpp"

class GamepadController {

public:
    GamepadController();

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

    void handleUpdateControllerState(SDL_Event &event, ControllerState *controllerState);

    void spamController(double deltaTime);

private:
    bool isLeftIn = false, isRightIn = false, prevIsLeftIn = false, prevIsRightIn = false, isRightSpamming = false, isLeftSpamming = false, isLeftFirstIn = false, isRightFirstIn = false;

    bool keyState[1] = {false};

    int lastAxisZeroValue = 0;
    double currentSpamTime = 0;
};