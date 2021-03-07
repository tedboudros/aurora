#include <SDL2/SDL.h>
#include <SDL2/SDL_image.h>
#include <iostream>

#include "RenderWindow.hpp"

int main(int argc, char* args[]) {
	if (SDL_Init(SDL_INIT_VIDEO) > 0) 
		std::cout << "ERROR: SDL_Init has failed. SDL_ERROR: " << SDL_GetError() << std::endl;
	if (!(IMG_Init(IMG_INIT_PNG))) 
		std::cout << "ERROR: IMG_Init has failed. IMG_ERROR: " << SDL_GetError() << std::endl;
	
	RenderWindow window("Aurora v0.0.1", 1280, 720);

	SDL_Texture* auroraLogo = window.loadTexture("res/images/logo.png");

	bool isGameRunning = true;
	SDL_Event event;
	// Game loop:
	while(isGameRunning) {

		// SDL Events:
		while(SDL_PollEvent(&event)){
			if(event.type == SDL_QUIT){
				isGameRunning = false;
			}
		}

		window.clear();
		window.render(auroraLogo);
		window.display();

	}


	window.cleanUp();
	SDL_Quit();

	return 0;
}