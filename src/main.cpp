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
	
	RenderWindow window("Aurora v0.0.2", 1920, 1080);

	//window.setFullScreen(true);

	int windowRefreshRate = window.getRefreshRate();

	std::cout << "Window Info:" << std::endl << "Current display refresh rate: " << windowRefreshRate << std::endl;

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

	int selectedGame = 0;
	int prevSelectedGame = 0;

	Vector2f windowSize = window.getWindowDimensions();
	std::cout << "Width: " << windowSize.x << ", Height: " << windowSize.y << std::endl << std::endl;
	Uint64 currentTime = utils::hireTime();

	GamepadDirectionEvent gamepadEvents;

	int lastLeftRightAxisValue = 0;
	const int axisThreshold = 20000;

	// Game loop:
	while(isGameRunning) {
		Uint64 newTime = utils::hireTime();
		double deltaTime = ((newTime - currentTime)*1000 / (double)SDL_GetPerformanceFrequency() );
		currentTime = newTime;

		std::cout << "DELTA TIME: " << deltaTime << std::endl;

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
					    		//std::cout << "Right In" << std::endl;

					    		if(selectedGame != (static_cast<int>(games.size()) -1)) {
					    			selectedGame += 1;
					    		}
					    	}
					    	if(lastLeftRightAxisValue > axisThreshold && event.jaxis.value <= axisThreshold){
					    		// Right Out
					    		//std::cout << "Right Out" << std::endl;
					    	}

					    	if(lastLeftRightAxisValue > -axisThreshold && event.jaxis.value <= -axisThreshold){
					    		// Left Out
					    		//std::cout << "Left In" << std::endl;

					    		if(selectedGame != 0) {
					    			selectedGame -= 1;
					    		}
					    	}
					    	if(lastLeftRightAxisValue <= -axisThreshold && event.jaxis.value > -axisThreshold){
					    		// Left In
					    		//std::cout << "Left Out" << std::endl;
					    	}

					    	lastLeftRightAxisValue = event.jaxis.value;
					    }
					}

					if(selectedGame != prevSelectedGame) {
						std::cout << "Selected Game: " << selectedGame << std::endl;

						games[selectedGame].setW(110);
						games[selectedGame].setH(110);

						games[prevSelectedGame].setW(100);
						games[prevSelectedGame].setH(100);

						prevSelectedGame = selectedGame;
					}

					break;
			}
		}

		// Actual Rendering
		window.clear();

		for(Entity& game : games) {
			window.render(game);
		}

		window.display();

	}

	window.cleanUp();
	SDL_Quit();

	return 0;
}