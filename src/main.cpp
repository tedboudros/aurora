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
	if (!(IMG_Init(IMG_INIT_PNG | IMG_INIT_JPG))) 
		std::cout << "ERROR: IMG_Init has failed. IMG_ERROR: " << SDL_GetError() << std::endl;
	
	RenderWindow window("Aurora v0.0.5", 1600, 900);

	//window.setFullScreen(true);

	int windowRefreshRate = window.getRefreshRate();

	std::cout << "Window Info:" << std::endl << "Current display refresh rate: " << windowRefreshRate << std::endl;

	SDL_Texture* gameBorder = window.loadTexture("res/images/game.png");
	SDL_Texture* wallpaper = window.loadTexture("res/images/wallpaper.jpg");

	std::vector<Entity> games = {};

	// CONSTANTS HERE FOR NOW:
	const int gameSizeNormal = 128;
	const int gameSizeSelected = 136;
	const int selectedGameOffset = 4;
	const int generalGameOffset = 48;
	const int marginBetweenGames = 16;
	const int normalY = 24;
	const int selectedYOffset = 16;
	const int transitionTime = 130;
	
	for (int i = 0; i < 10; i++) {
		{
			Entity anotherGame(i == 0 ? Vector4f(generalGameOffset - selectedGameOffset, normalY+selectedYOffset, gameSizeSelected, gameSizeSelected) : Vector4f(i*(gameSizeNormal + (marginBetweenGames * 2)) + generalGameOffset,normalY, gameSizeNormal, gameSizeNormal), gameBorder);
			games.push_back(anotherGame);
		}
	}

	bool isGameRunning = true;
	SDL_Event event;

	int selectedGame = 1;
	int prevSelectedGame = 0;

	Vector2f windowSize = window.getWindowDimensions();
	std::cout << "Width: " << windowSize.x << ", Height: " << windowSize.y << std::endl << std::endl;

	
	Entity wallpaperEntity(Vector4f(0,0,windowSize.x,windowSize.y), wallpaper);
	

	Uint64 currentTime = utils::hireTime();

	GamepadDirectionEvent gamepadEvents;

	int lastLeftRightAxisValue = 0;
	const int axisThreshold = 20000;

	// Game loop:
	while(isGameRunning) {
		Uint64 newTime = utils::hireTime();
		double deltaTime = ((newTime - currentTime)*1000 / (double)SDL_GetPerformanceFrequency() );
		currentTime = newTime;

		//std::cout << "DELTA TIME: " << deltaTime << std::endl;

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

					break;
			}
		}

		if(selectedGame != prevSelectedGame) {
			// std::cout << "Selected Game: " << selectedGame << std::endl;

			for(int i = 0; i < static_cast<int>(games.size()); i++) {
				int newX,newY,newW,newH;

				newX = (i*(gameSizeNormal + (marginBetweenGames * 2))) - selectedGame * (gameSizeNormal + (marginBetweenGames * 2)) + generalGameOffset;
				newY = normalY;
				newW = gameSizeNormal;
				newH = gameSizeNormal;

				if(i == selectedGame) {
					newX -= selectedGameOffset;
					newY += selectedYOffset;
					newW = gameSizeSelected;
					newH = gameSizeSelected;
				}

				games[i].setAnimation(Vector4f(newX, newY, newW, newH), transitionTime);
			}

			prevSelectedGame = selectedGame;
		}

		// Actual Rendering
		window.clear();

		window.render(wallpaperEntity);

		for(Entity& game : games) {
			game.animate(deltaTime);
			window.render(game);
		}

		window.display();

	}

	window.cleanUp();
	SDL_Quit();

	return 0;
}