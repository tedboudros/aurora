#pragma once

#include <SDL2/SDL.h>
#include <SDL2/SDL_image.h>

#include "Math.hpp"
#include "Animation/Easing.h"

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

	void execFrame(SDL_Event& event);

	void spamController(double deltaTime);

private:
	bool isLeftIn = false, isRightIn = false, prevIsLeftIn = false, prevIsRightIn = false, isRightSpamming = false, isLeftSpamming = false, isLeftFirstIn = false, isRightFirstIn = false;
	int lastAxisZeroValue = 0;
	double currentSpamTime = 0;
};