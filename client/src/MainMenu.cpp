#include <iostream>

#include "RenderWindow.hpp"
#include "Entity.hpp"
#include "Math.hpp"
#include "Pages/MainMenu.hpp"
#include <Parsers/JSON.hpp>
#include "HTTPRequest.hpp"

// for convenience
using json = nlohmann::json;

MainMenuPage::MainMenuPage(RenderWindow* p_window, int p_server_port) :window(p_window), server_port(p_server_port)  {
	SDL_Texture* wallpaper = window->loadTexture("res/images/bg.png");
	wallpaperEntity = Entity(MultiSize(Size(0, SIZE_WIDTH),Size(0, SIZE_HEIGHT),Size(100, SIZE_WIDTH),Size(100, SIZE_HEIGHT)), wallpaper);

    
	this->readGameStyles();
	this->requestSteamGamesFromServer();
};

void MainMenuPage::render(double deltaTime) {
	window->render(wallpaperEntity);
};

void MainMenuPage::createGameEntity(int i, std::string name) {
    const MultiSize normalGameDims(Size(i*(gameSizeNormal + (marginBetweenGames * 2) + selectedGameOffset) + gameOffset, SIZE_HEIGHT), Size(normalY, SIZE_HEIGHT), Size(gameSizeNormal, SIZE_HEIGHT), Size(gameSizeNormal, SIZE_HEIGHT) );
    const MultiSize selectedGameDims(Size(gameOffset - selectedGameOffset, SIZE_HEIGHT), Size(normalY, SIZE_HEIGHT), Size(gameSizeSelected, SIZE_HEIGHT), Size(gameSizeSelected, SIZE_HEIGHT) );
    
    Entity anotherGame(i == 0 ? selectedGameDims : normalGameDims, gameBorder);

    gameEntities.push_back(anotherGame);
    gameNames.push_back(name);
};

void MainMenuPage::readGameStyles() {	
    std::ifstream gameStylesJSON("res/styles/game.json");
    json gameStyles;
    gameStylesJSON >> gameStyles;

    gameSizeNormal = gameStyles["game"]["normal"]["size"]["value"];
    gameSizeSelected = gameStyles["game"]["active"]["size"]["value"];
    selectedGameOffset = (gameSizeSelected - gameSizeNormal);
   // gameFontFamilyName = gameStyles["game"]["font"]["filename"];
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

void MainMenuPage::requestSteamGamesFromServer() {
    try {
        http::Request request{"http://127.0.0.1:" + std::string(args[1]) + "/steam"};

        const auto response = request.send("GET");
        const std::string response_str = std::string{response.body.begin(), response.body.end()};

        auto gamesResponse = json::parse(response_str);
        std::cout << "Got " << gamesResponse.size() << " games from the server!" <<  std::endl;

        for (int i = 0; i < static_cast<int>(gamesResponse.size()); i++) {
            this->createGameEntity(i, gamesResponse[i]["name"]);
        }
    }
    catch (const std::exception& e) {
        std::cerr << "Request failed, error: " << e.what() << '\n';
    }
};