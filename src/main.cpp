#include <SDL2/SDL.h>
#include <SDL2/SDL_image.h>
#include <iostream>
#include <vector>

#include "RenderWindow.hpp"
#include "Entity.hpp"
#include "Math.hpp"
#include "Utils.hpp"

int main(int argc, char* args[]) {
	if (SDL_Init(SDL_INIT_VIDEO) > 0) 
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
				if(event.type == SDL_QUIT){
					isGameRunning = false;
				}
			}


			accumulator -= deltaTime;
		}

		std::cout << utils::hireTimeInSeconds() << std::endl;

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