#include <SDL2/SDL.h>
#include <SDL2/SDL_image.h>
#include <SDL2/SDL_ttf.h>
#include <iostream>
#include <fstream>
#include <vector>

#include "RenderWindow.hpp"
#include "Entity.hpp"
#include "Text.hpp"
#include "Math.hpp"
#include "Utils.hpp"
#include "Gamepad/GamepadController.hpp"
#include <Parsers/JSON.hpp>

// for convenience
using json = nlohmann::json;

int main(int argc, char* args[]) {
	if (SDL_Init(SDL_INIT_VIDEO | SDL_INIT_JOYSTICK) < 0) 
		std::cout << "ERROR: SDL_Init has failed. SDL_ERROR: " << SDL_GetError() << std::endl;
	if (!(IMG_Init(IMG_INIT_PNG | IMG_INIT_JPG))) 
		std::cout << "ERROR: IMG_Init has failed. IMG_ERROR: " << SDL_GetError() << std::endl;
	
	RenderWindow window("Aurora v0.0.7", 1920, 1080);

	//window.setFullScreen(true);

	int windowRefreshRate = window.getRefreshRate();

	std::cout << "Window Info:" << std::endl << "Current display refresh rate: " << windowRefreshRate << std::endl;

	SDL_Texture* gameBorder = window.loadTexture("res/images/game.png");
	SDL_Texture* wallpaper = window.loadTexture("res/images/bg.png");

	std::vector<Entity> games = {};


	std::ifstream gameStylesJSON("res/styles/game.json");
	json gameStyles;
	gameStylesJSON >> gameStyles;

	float gameSizeNormal, gameSizeSelected, selectedGameOffset, gameOffset, marginBetweenGames, normalY;
	int normalTransitionTime, spamTransitionTime;
	std::string gameFontFName;
	int gameFontSize = 10;

	auto readGameStyles = [&] () {	
		std::ifstream gameStylesJSON("res/styles/game.json");
		json gameStyles;
		gameStylesJSON >> gameStyles;

		gameSizeNormal = gameStyles["game"]["normal"]["size"]["value"];
		gameSizeSelected = gameStyles["game"]["active"]["size"]["value"];
		selectedGameOffset = (gameSizeSelected - gameSizeNormal);
		gameFontFName = gameStyles["game"]["font"]["filename"];
		gameFontSize = gameStyles["game"]["font"]["size"];
		gameOffset = gameStyles["game-container"]["x"]["value"];
		marginBetweenGames = gameStyles["game-container"]["spacing"]["value"];
		normalY = gameStyles["game-container"]["y"]["value"];
		normalTransitionTime = gameStyles["game-container"]["normalTransition"];
		spamTransitionTime = gameStyles["game-container"]["spamTransition"];
	};

	readGameStyles();

	// CONSTANTS HERE FOR NOW:
	
	for (int i = 0; i < 20; i++) {
		{
			const MultiSize normalGameDims(Size(i*(gameSizeNormal + (marginBetweenGames * 2) + selectedGameOffset) + gameOffset, SIZE_HEIGHT), Size(normalY, SIZE_HEIGHT), Size(gameSizeNormal, SIZE_HEIGHT), Size(gameSizeNormal, SIZE_HEIGHT) );

			const MultiSize selectedGameDims(Size(gameOffset - selectedGameOffset, SIZE_HEIGHT), Size(normalY, SIZE_HEIGHT), Size(gameSizeSelected, SIZE_HEIGHT), Size(gameSizeSelected, SIZE_HEIGHT) );

			Entity anotherGame(i == 0 ? selectedGameDims : normalGameDims, gameBorder);
			games.push_back(anotherGame);
		}
	}
	
	Text sampleText1, sampleText2, sampleText3;
	TTF_Font *font = NULL;
	if (TTF_Init() < 0) {
		std::cout << "TTF init has failed. TTF error: " << TTF_GetError() << std::endl;
	} else {
		std::cout << "gameFontFName: " << gameFontFName << ", size: " << gameFontSize << std::endl;
		font = TTF_OpenFont(gameFontFName.c_str(), gameFontSize);
		if (font) {
			std::cout << "Font opened." << std::endl;
			// sampleText1
			sampleText1.setFont(font);
			sampleText1.setColor(SDL_Color{255, 255, 255, 255});
			sampleText1.setText("Sample Text (Solid Method)");
			sampleText1.setRenderMethod(Text::RenderMethod::Solid);
			sampleText1.setDestPos(Size(5, SIZE_WIDTH), Size(50, SIZE_HEIGHT));
			//sampleText1.setDestRect(MultiSize(Size(5, SIZE_WIDTH), Size(50, SIZE_HEIGHT), Size(15, SIZE_WIDTH), Size(5, SIZE_HEIGHT)));
			//sampleText1.setUseTextureSize(false);
			// sampleText2
			sampleText2.setFont(font);
			sampleText2.setColor(SDL_Color{255, 255, 255, 255});
			sampleText2.setBackGndColor(SDL_Color{64, 140, 196, 255});
			sampleText2.setText("Sample Text (Shaded Method)");
			sampleText2.setRenderMethod(Text::RenderMethod::Shaded);
			sampleText2.setDestPos(Size(5, SIZE_WIDTH), Size(55, SIZE_HEIGHT));
			// sampleText3
			sampleText3.setFont(font);
			sampleText3.setColor(SDL_Color{255, 255, 255, 255});
			sampleText3.setText("Sample Text (Blended Method)");
			sampleText3.setRenderMethod(Text::RenderMethod::Blended);
			sampleText3.setDestPos(Size(5, SIZE_WIDTH), Size(60, SIZE_HEIGHT));
		} else
			std::cout << "Font not opened. TTF error: " << TTF_GetError() << std::endl;
	}

	bool isGameRunning = true;
	SDL_Event event;

	int selectedGame = 0;
	int prevSelectedGame = 0;

	Vector2f windowSize = window.getWindowDimensions();
	std::cout << "Width: " << windowSize.x << ", Height: " << windowSize.y << std::endl << std::endl;

	
	Entity wallpaperEntity(MultiSize(Size(0, SIZE_WIDTH),Size(0, SIZE_HEIGHT),Size(100, SIZE_WIDTH),Size(100, SIZE_HEIGHT)), wallpaper);
	

	Uint64 currentTime = utils::hireTime();

	GamepadController gamepadController;

	int lastLeftRightAxisValue = 0;
	const int axisThreshold = 20000;

	bool isLeft = false, isRight = false, isSpamming = false;


	auto animateGames = [&] () {
		for(int i = 0; i < static_cast<int>(games.size()); i++) {
			int newX,newY,newW,newH;

			newX = (i*(gameSizeNormal + (marginBetweenGames * 2))) - selectedGame * (gameSizeNormal + (marginBetweenGames * 2)) + gameOffset;
			newY = normalY;
			newW = gameSizeNormal;
			newH = gameSizeNormal;

			if(i == selectedGame) {
				newW = gameSizeSelected;
				newH = gameSizeSelected;
			}

			if(i > selectedGame) {
				newX += selectedGameOffset;
			}

			games[i].setAnimation(MultiSize(Size(newX, SIZE_HEIGHT), Size(newY, SIZE_HEIGHT), Size(newW, SIZE_HEIGHT), Size(newH, SIZE_HEIGHT)), isSpamming ? spamTransitionTime : normalTransitionTime);
		}
	};

	animateGames();

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
					readGameStyles();
					animateGames();
					//isGameRunning = false;
					break;

				case SDL_JOYAXISMOTION:
					gamepadController.execFrame(event);
					if(gamepadController.onLeft()) {
						isSpamming = false;

			    		if(selectedGame != 0) {
			    			selectedGame -= 1;
			    		}
					}else if(gamepadController.onRight()){		
						isSpamming = false;		

			    		if(selectedGame != (static_cast<int>(games.size()) -1)) {
			    			selectedGame += 1;
			    		}
					}

					break;
			}
		}

		gamepadController.spamController(deltaTime);

		if(gamepadController.onLeftSpam()) {
			isSpamming = true;

    		if(selectedGame != 0) {
    			selectedGame -= 1;
    		}
		}else if(gamepadController.onRightSpam()){		
			isSpamming = true;

    		if(selectedGame != (static_cast<int>(games.size()) -1)) {
    			selectedGame += 1;
    		}
		}

		if(selectedGame != prevSelectedGame) {
			animateGames();
			prevSelectedGame = selectedGame;
		}

		// Actual Rendering
		window.clear();

		window.render(wallpaperEntity);

		for(Entity& game : games) {
			game.animate(deltaTime);
			window.render(game);
		}
		
		if (font) {
			window.render(sampleText1);
			window.render(sampleText2);
			window.render(sampleText3);
		}

		window.display();

	}

	TTF_CloseFont(font);
	TTF_Quit();
	window.cleanUp();
	SDL_Quit();

	return 0;
}