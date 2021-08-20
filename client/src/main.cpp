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
#include "Keyboard/KeyboardController.hpp"
#include <Parsers/JSON.hpp>
#include "HTTPRequest.hpp"

// for convenience
using json = nlohmann::json;

int main(int argc, char* args[]) {
	if (SDL_Init(SDL_INIT_VIDEO | SDL_INIT_JOYSTICK) < 0) 
		std::cout << "ERROR: SDL_Init has failed. SDL_ERROR: " << SDL_GetError() << std::endl;
	if (!(IMG_Init(IMG_INIT_PNG | IMG_INIT_JPG))) 
		std::cout << "ERROR: IMG_Init has failed. IMG_ERROR: " << SDL_GetError() << std::endl;		
	if(!TTF_WasInit() && TTF_Init() == -1) {
		std::cout << "ERROR: TTF_Init has failed. TTF_ERROR: " << TTF_GetError() << std::endl;	
	}

	RenderWindow window("Aurora", 1600, 900);

	int windowRefreshRate = window.getRefreshRate();

	std::cout << "Window Info:" << std::endl << "Current display refresh rate: " << windowRefreshRate << std::endl;

	SDL_Texture* gameBorder = window.loadTexture("res/images/game.png");
	SDL_Texture* wallpaper = window.loadTexture("res/images/bg.png");

	std::vector<Entity> gameEntities = {};
	std::vector<std::string> gameNames = {};

	std::ifstream gameStylesJSON("res/styles/game.json");
	json gameStyles;
	gameStylesJSON >> gameStyles;

	float gameSizeNormal, gameSizeSelected, selectedGameOffset, gameOffset, marginBetweenGames, normalY, gameTitleFontScale, gameTitleX, gameTitleY;
	int normalTransitionTime, spamTransitionTime, gameTitleFontSize;
	std::string gameFontFamilyName;

	auto createGameEntity = [&] (int i, std::string name) {
		const MultiSize normalGameDims(Size(i*(gameSizeNormal + (marginBetweenGames * 2) + selectedGameOffset) + gameOffset, SIZE_HEIGHT), Size(normalY, SIZE_HEIGHT), Size(gameSizeNormal, SIZE_HEIGHT), Size(gameSizeNormal, SIZE_HEIGHT) );
		const MultiSize selectedGameDims(Size(gameOffset - selectedGameOffset, SIZE_HEIGHT), Size(normalY, SIZE_HEIGHT), Size(gameSizeSelected, SIZE_HEIGHT), Size(gameSizeSelected, SIZE_HEIGHT) );
		
		Entity anotherGame(i == 0 ? selectedGameDims : normalGameDims, gameBorder);
		gameEntities.push_back(anotherGame);

		gameNames.push_back(name);
	};

	auto requestSteamGamesFromServer = [&] () {
		try {
			http::Request request{"http://127.0.0.1:" + std::string(args[1]) + "/steam"};

			const auto response = request.send("GET");
			const std::string response_str = std::string{response.body.begin(), response.body.end()};

			auto gamesResponse = json::parse(response_str);
			std::cout << "Got " << gamesResponse.size() << " games from the server!" <<  std::endl;

			for (int i = 0; i < gamesResponse.size(); i++) {
				createGameEntity(i, gamesResponse[i]["name"]);
			}
		}
		catch (const std::exception& e) {
			std::cerr << "Request failed, error: " << e.what() << '\n';
		}
	};


	auto readGameStyles = [&] () {	
		std::ifstream gameStylesJSON("res/styles/game.json");
		json gameStyles;
		gameStylesJSON >> gameStyles;

		gameSizeNormal = gameStyles["game"]["normal"]["size"]["value"];
		gameSizeSelected = gameStyles["game"]["active"]["size"]["value"];
		selectedGameOffset = (gameSizeSelected - gameSizeNormal);
		gameFontFamilyName = gameStyles["game"]["font"]["filename"];
		gameTitleFontSize = gameStyles["game"]["font"]["size"];
		gameTitleFontScale = gameStyles["game"]["font"]["scale"];
		gameTitleX = gameStyles["game"]["font"]["x"];
		gameTitleY = gameStyles["game"]["font"]["y"];
		gameOffset = gameStyles["game-container"]["x"]["value"];
		marginBetweenGames = gameStyles["game-container"]["spacing"]["value"];
		normalY = gameStyles["game-container"]["y"]["value"];
		normalTransitionTime = gameStyles["game-container"]["normalTransition"];
		spamTransitionTime = gameStyles["game-container"]["spamTransition"];
	};

	readGameStyles();
	requestSteamGamesFromServer();

	// CONSTANTS HERE FOR NOW:

	Text gameTitle;
	TTF_Font *font = NULL;
	
	bool isGameRunning = true;
	SDL_Event event;

	int selectedGame = 0;
	int prevSelectedGame = 0;

	Vector2f windowSize = window.getWindowDimensions();
	std::cout << "Width: " << windowSize.x << ", Height: " << windowSize.y << std::endl;

	auto setGameTitleFont = [&] () { 
		font = TTF_OpenFont(gameFontFamilyName.c_str(), gameTitleFontSize);

		gameTitle.setFont(font);
		gameTitle.setColor(SDL_Color{255, 255, 255, 255});
		if(gameNames.size() >= 1) {
			gameTitle.setText(gameNames[selectedGame]);
		}
		gameTitle.setFontScale(gameTitleFontScale);
		gameTitle.setRenderMethod(Text::RenderMethod::Blended);
		gameTitle.setPosition(Size(gameTitleX, SIZE_WIDTH), Size(gameTitleY, SIZE_HEIGHT));
	};

	setGameTitleFont();
	
	Entity wallpaperEntity(MultiSize(Size(0, SIZE_WIDTH),Size(0, SIZE_HEIGHT),Size(100, SIZE_WIDTH),Size(100, SIZE_HEIGHT)), wallpaper);
	

	Uint64 currentTime = utils::hireTime();

	GamepadController gamepadController;
	KeyboardController keyboardController;

	int lastLeftRightAxisValue = 0;
	const int axisThreshold = 20000;

	bool isLeft = false, isRight = false, isSpamming = false;


	auto animateGames = [&] () {
		for(int i = 0; i < static_cast<int>(gameEntities.size()); i++) {
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

			gameEntities[i].setAnimation(MultiSize(Size(newX, SIZE_HEIGHT), Size(newY, SIZE_HEIGHT), Size(newW, SIZE_HEIGHT), Size(newH, SIZE_HEIGHT)), isSpamming ? spamTransitionTime : normalTransitionTime);
		}
	};

	animateGames();

	
	auto navigateLeft = [&] () {
		isSpamming = false;

		if(selectedGame != 0) {
			selectedGame -= 1;
		}
	};

	auto navigateRight = [&] () {
		isSpamming = false;		

		if(selectedGame != (static_cast<int>(gameEntities.size()) -1)) {
			selectedGame += 1;
		}
	};
	
	auto spamLeft = [&] () {
		isSpamming = true;

		if(selectedGame != 0) {
			selectedGame -= 1;
		}
	};

	auto spamRight = [&] () {
		isSpamming = true;

		if(selectedGame != (static_cast<int>(gameEntities.size()) -1)) {
			selectedGame += 1;
		}
	};

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
					// readGameStyles();
					// animateGames();
					// setGameTitleFont();	
					isGameRunning = false;
					break;

				case SDL_JOYAXISMOTION:
					gamepadController.execFrame(event);
					if(gamepadController.onLeft()) {
						navigateLeft();
					}else if(gamepadController.onRight()){		
						navigateRight();
					}

					break;

				case SDL_KEYDOWN:
					keyboardController.execFrame(event);
					
					if(keyboardController.onLeft()) {
						navigateLeft();
					}else if(keyboardController.onRight()){		
						navigateRight();
					}

					break;

				case SDL_KEYUP:
					keyboardController.execFrame(event);
					break;
			}
		}

		gamepadController.spamController(deltaTime);
		keyboardController.spamKeyboard(deltaTime);

		if(gamepadController.onLeftSpam() || keyboardController.onLeftSpam()) {
			spamLeft();
		}else if(gamepadController.onRightSpam() || keyboardController.onRightSpam()){	
			spamRight();
		}

		if(selectedGame != prevSelectedGame) {
			animateGames();	
			setGameTitleFont();
			prevSelectedGame = selectedGame;
		}

		// Actual Rendering
		window.clear();

		window.render(wallpaperEntity);

		for(Entity& game : gameEntities) {
			game.animate(deltaTime);
			window.render(game);
		}
		
		if (font) {
			window.render(gameTitle);
		}

		window.display();

	}

	TTF_CloseFont(font);
	TTF_Quit();
	window.cleanUp();
	SDL_Quit();

	return 0;
}