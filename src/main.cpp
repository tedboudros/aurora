#include <SDL2/SDL.h>
#include <SDL2/SDL_image.h>
#include <iostream>
#include <vector>

#include "RenderWindow.hpp"
#include "Entity.hpp"
#include "Math.hpp"
#include "Utils.hpp"
#include "Gamepad/GamepadDirectionEvent.hpp"

int main(int argc, char* args[]) {
	if (SDL_Init(SDL_INIT_VIDEO | SDL_INIT_JOYSTICK) < 0) 
		std::cout << "ERROR: SDL_Init has failed. SDL_ERROR: " << SDL_GetError() << std::endl;
	if (!(IMG_Init(IMG_INIT_PNG))) 
		std::cout << "ERROR: IMG_Init has failed. IMG_ERROR: " << SDL_GetError() << std::endl;
	
	RenderWindow window("Aurora v0.0.1", 1600, 900);
	int windowRefreshRate = window.getRefreshRate();

	std::cout << "Window refresh rate: " << windowRefreshRate << std::endl;

	SDL_Texture* gameBorder = window.loadTexture("res/images/gameBorder.png");

	std::vector<Entity> games = {};

	for (int i = 0; i < 10; i++) {
		{
			Entity anotherGame(Vector4f(i*125 + 10,10, 100, 100), gameBorder);
			games.push_back(anotherGame);
		}
	}

	bool isGameRunning = true;
	SDL_Event event;

	const float deltaTime = 0.01f;
	float accumulator = 0.0f;
	float currentTime = utils::hireTimeInSeconds();

	GamepadDirectionEvent gamepadEvents;

	int lastLeftRightAxisValue = 0;
	const int axisThreshold = 20000;

	// Game loop:
	while(isGameRunning) {

		int startTicks = SDL_GetTicks();

		float newTime = utils::hireTimeInSeconds();
		float frameTime = newTime - currentTime;
		currentTime = newTime;

		accumulator += frameTime;

		while(accumulator >= deltaTime) {
			// SDL Events:
			while(SDL_PollEvent(&event)){
				switch(event.type) {
					case SDL_QUIT:
						isGameRunning = false;
						break;

					case SDL_JOYAXISMOTION:
						if (( event.jaxis.value < -3200 ) || (event.jaxis.value > 3200 )) {
							if( event.jaxis.axis == 0) {
						        // Left-right movement code goes here
						
						    	if(lastLeftRightAxisValue <= axisThreshold && event.jaxis.value > axisThreshold){
						    		// Right In
						    		std::cout << "Right In" << std::endl;
						    	}
						    	if(lastLeftRightAxisValue > axisThreshold && event.jaxis.value <= axisThreshold){
						    		// Right Out
						    		std::cout << "Right Out" << std::endl;
						    	}

						    	if(lastLeftRightAxisValue > -axisThreshold && event.jaxis.value <= -axisThreshold){
						    		// Left Out
						    		std::cout << "Left In" << std::endl;
						    	}
						    	if(lastLeftRightAxisValue <= -axisThreshold && event.jaxis.value > -axisThreshold){
						    		// Left In
						    		std::cout << "Left Out" << std::endl;
						    	}

						    	lastLeftRightAxisValue = event.jaxis.value;
						    }
						}
						break;
				}
			}


			accumulator -= deltaTime;
		}

		window.clear();

		for(Entity& game : games) {
			window.render(game);
		}

		window.display();

		int frameTicks = SDL_GetTicks() - startTicks;

		if(frameTicks < 1000 / window.getRefreshRate()) {
			SDL_Delay(1000 / window.getRefreshRate() - frameTicks);
		}

	}


	window.cleanUp();
	SDL_Quit();

	return 0;
}