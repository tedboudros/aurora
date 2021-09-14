#pragma once

#include <SDL2/SDL.h>
#include <SDL2/SDL_image.h>
#include <iostream>
#include <vector>

#include "Utilities/Math/Math.hpp"
#include "Rendering/Easing/Easing.h"

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

	void execFrame(SDL_Event& event);
	void spamKeyboard(double deltaTime);

private:
	bool isLeftIn = false, isRightIn = false, prevIsLeftIn = false, prevIsRightIn = false, isRightSpamming = false, isLeftSpamming = false, isLeftFirstIn = false, isRightFirstIn = false;
    const Uint8* keyboardKeys = SDL_GetKeyboardState(NULL);
	double currentSpamTime = 0;
};