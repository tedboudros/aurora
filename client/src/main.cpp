#include <SDL2/SDL.h>
#include <SDL2/SDL_image.h>
#include <SDL2/SDL_ttf.h>
#include <iostream>
#include <fstream>
#include <vector>

#include "RenderWindow.hpp"
#include "Pages/MainMenu.hpp"

#include "Entity.hpp"
#include "Text.hpp"
#include "Math.hpp"
#include "Utils.hpp"
#include "Gamepad/GamepadController.hpp"
#include "Keyboard/KeyboardController.hpp"
#include <Parsers/JSON.hpp>
#include "Server.hpp"

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

	// Text gameTitle;
	// TTF_Font *font = NULL;

	// auto setGameTitleFont = [&] () { 
	// 	font = TTF_OpenFont(gameFontFamilyName.c_str(), gameTitleFontSize);

	// 	gameTitle.setFont(font);
	// 	gameTitle.setColor(SDL_Color{255, 255, 255, 255});
	// 	if(gameNames.size() >= 1) {
	// 		gameTitle.setText(gameNames[selectedGame]);
	// 	}
	// 	gameTitle.setFontScale(gameTitleFontScale);
	// 	gameTitle.setRenderMethod(Text::RenderMethod::Blended);
	// 	gameTitle.setPosition(Size(gameTitleX, SIZE_WIDTH), Size(gameTitleY, SIZE_HEIGHT));
	// };

	// setGameTitleFont();

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
		
		// if (font) {
		// 	window.render(gameTitle);
		// }

		window.display();

	}

	// TTF_CloseFont(font);
	// TTF_Quit();
	window.cleanUp();
	SDL_Quit();

	return 0;
}