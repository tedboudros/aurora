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
	
	RenderWindow window("Aurora v0.0.5", 1920, 1000);

	//window.setFullScreen(true);

	int windowRefreshRate = window.getRefreshRate();

	std::cout << "Window Info:" << std::endl << "Current display refresh rate: " << windowRefreshRate << std::endl;

	SDL_Texture* gameBorder = window.loadTexture("res/images/game.png");
	SDL_Texture* wallpaper = window.loadTexture("res/images/wallpaper.jpg");

	std::vector<Entity> games = {};

	// CONSTANTS HERE FOR NOW:
	const float gameSizeNormal = 12;
	const float gameSizeSelected = 15;
	const float selectedGameOffset = (gameSizeSelected - gameSizeNormal);
	const float gameOffset = 3;
	const float marginBetweenGames = 1;
	const float normalY = 100 - gameSizeNormal - 2;
	const int transitionTime = 135;
	
	for (int i = 0; i < 20; i++) {
		{
			const MultiSize normalGameDims(Size(i*(gameSizeNormal + (marginBetweenGames * 2) + selectedGameOffset) + gameOffset, SIZE_HEIGHT), Size(normalY, SIZE_HEIGHT), Size(gameSizeNormal, SIZE_HEIGHT), Size(gameSizeNormal, SIZE_HEIGHT) );

			const MultiSize selectedGameDims(Size(gameOffset - selectedGameOffset, SIZE_HEIGHT), Size(normalY+selectedGameOffset, SIZE_HEIGHT), Size(gameSizeSelected, SIZE_HEIGHT), Size(gameSizeSelected, SIZE_HEIGHT) );

			Entity anotherGame(i == 0 ? selectedGameDims : normalGameDims, gameBorder);
			games.push_back(anotherGame);
		}
	}

	bool isGameRunning = true;
	SDL_Event event;

	int selectedGame = 1;
	int prevSelectedGame = 0;

	Vector2f windowSize = window.getWindowDimensions();
	std::cout << "Width: " << windowSize.x << ", Height: " << windowSize.y << std::endl << std::endl;

	
	Entity wallpaperEntity(MultiSize(Size(0, SIZE_WIDTH),Size(0, SIZE_HEIGHT),Size(100, SIZE_WIDTH),Size(100, SIZE_HEIGHT)), wallpaper);
	

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

					    		if(selectedGame != (static_cast<int>(games.size()) -1)) {
					    			selectedGame += 1;
					    		}
					    	}
					    	if(lastLeftRightAxisValue > axisThreshold && event.jaxis.value <= axisThreshold){
					    		// Right Out
					    	}

					    	if(lastLeftRightAxisValue > -axisThreshold && event.jaxis.value <= -axisThreshold){
					    		// Left Out

					    		if(selectedGame != 0) {
					    			selectedGame -= 1;
					    		}
					    	}
					    	if(lastLeftRightAxisValue <= -axisThreshold && event.jaxis.value > -axisThreshold){
					    		// Left In
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

				newX = (i*(gameSizeNormal + (marginBetweenGames * 2))) - selectedGame * (gameSizeNormal + (marginBetweenGames * 2)) + gameOffset;
				newY = normalY;
				newW = gameSizeNormal;
				newH = gameSizeNormal;

				if(i == selectedGame) {
					newY -= selectedGameOffset;
					newW = gameSizeSelected;
					newH = gameSizeSelected;
				}

				if(i > selectedGame) {
					newX += selectedGameOffset;
				}

				games[i].setAnimation(MultiSize(Size(newX, SIZE_HEIGHT), Size(newY, SIZE_HEIGHT), Size(newW, SIZE_HEIGHT), Size(newH, SIZE_HEIGHT)), transitionTime);
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