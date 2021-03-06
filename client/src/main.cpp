#include <SDL2/SDL.h>
#include <iostream>

#include "Rendering/RenderWindow/RenderWindow.hpp"
#include "Pages/MainMenu/MainMenu.hpp"

#include "Utilities/Helpers/Helpers.hpp"

int main(int argc, char* args[]) {
 	bool isRunning = true;

	Server api(args[1]);
	
	RenderWindow window("Aurora", 1600, 900);

	MainMenuPage mainMenu(&window, &api);

	// Diagnostic stuff:
	int windowRefreshRate = window.getRefreshRate();
	std::cout << "Refresh rate: " << windowRefreshRate << "hz" << std::endl;

	Vector2f windowSize = window.getWindowDimensions();
	std::cout << "Resolution: " << windowSize.x << "x" << windowSize.y << std::endl;


	SDL_Event event;

	GamepadController gamepadController;
	KeyboardController keyboardController;

	// For delta time
	Uint64 currentTime = utils::hireTime();

	// Game loop:
	while(isRunning) {
		Uint64 newTime = utils::hireTime();
		double deltaTime = ((newTime - currentTime)*1000 / (double)SDL_GetPerformanceFrequency() );
		currentTime = newTime;

		// SDL Events:															
		while(SDL_PollEvent(&event)){
			switch(event.type) {
				case SDL_QUIT:
					isRunning = false;
					break;

				case SDL_JOYAXISMOTION:
					gamepadController.execFrame(event);
					mainMenu.executeControllerEvent(&gamepadController);
					break;

				case SDL_KEYDOWN:
					keyboardController.execFrame(event);
					mainMenu.executeKeyboardEvent(&keyboardController);
					break;

				case SDL_KEYUP:
					keyboardController.execFrame(event);
					break;
			}
		}

		gamepadController.spamController(deltaTime);
		keyboardController.spamKeyboard(deltaTime);

		mainMenu.executeSpamEvents(&gamepadController, &keyboardController);

		// Rendering
		window.clear();

		mainMenu.render(deltaTime);	

		window.display();

	}

	mainMenu.cleanUp();
	window.cleanUp();
	SDL_Quit();

	return 0;
}