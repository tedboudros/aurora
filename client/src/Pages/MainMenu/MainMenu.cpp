#include <iostream>
#include <fstream>

#include <SDL2/SDL_ttf.h>

#include "Rendering/RenderWindow/RenderWindow.hpp"

#include "Utilities/Sound/Sound.h"
#include "Utilities/Helpers/Helpers.hpp"

#include "Pages/MainMenu/MainMenu.hpp"

const std::string clockFormat = "%X";

MainMenuPage::MainMenuPage(RenderWindow *p_window, Server *p_api) : window(p_window), api(p_api) {
    SDL_Texture *wallpaper = window->loadTexture("res/images/bg.png");
    wallpaperEntity = Entity(
            MultiSize(Size(0, SIZE_WIDTH), Size(0, SIZE_HEIGHT), Size(100, SIZE_WIDTH), Size(100, SIZE_HEIGHT)),
            wallpaper);

    gameBorder = window->loadTexture("res/images/game.png");
    scrollSound = createAudio("res/sounds/scroll.wav", 0, SDL_MIX_MAXVOLUME);
    enterSound = createAudio("res/sounds/enter.wav", 0, SDL_MIX_MAXVOLUME);

    this->readGameStyles();
    this->requestSteamGamesFromServer();
    this->setGameTitleFont();
};

void MainMenuPage::render(double deltaTime) {
    if (selectedGame != prevSelectedGame) {
        playSoundFromMemory(scrollSound, SDL_MIX_MAXVOLUME / 4);
        this->animateGames();
        this->setGameTitleFont();

        prevSelectedGame = selectedGame;
    }

    clockText.setText(utils::currentDateTime(clockFormat));

    window->render(wallpaperEntity);

    window->render(gameTitle);
    window->render(clockText);

    for (Entity &game: gameEntities) {
        game.animate(deltaTime);
        window->render(game);
    }
}

void MainMenuPage::cleanUp() {

    freeAudio(scrollSound);

    TTF_CloseFont(gameTitleFont);
    TTF_CloseFont(clockTextFont);
    TTF_Quit();
}

void MainMenuPage::onRight() {
    isSpamming = false;

    if (selectedGame != (static_cast<int>(gameEntities.size()) - 1)) {
        selectedGame += 1;
    }
}

void MainMenuPage::onLeft() {
    isSpamming = false;

    if (selectedGame != 0) {
        selectedGame -= 1;
    }
}

void MainMenuPage::onRightSpam() {
    isSpamming = true;

    if (selectedGame != (static_cast<int>(gameEntities.size()) - 1)) {
        selectedGame += 1;
    }
}

void MainMenuPage::onLeftSpam() {
    isSpamming = true;

    if (selectedGame != 0) {
        selectedGame -= 1;
    }
}

void MainMenuPage::executeControllerState(ControllerState *p_controllerState) {
    const bool isEnterDown = p_controllerState->isDown(ControllerState::enter);
    if (isEnterDown) {
        // Launch game
        api->post("steam", json{{"app_id", games[selectedGame].steam_app_id}});
        // Play sound
        playSoundFromMemory(enterSound, SDL_MIX_MAXVOLUME / 6);

    }
}

void MainMenuPage::executeControllerEvent(GamepadController *gamepad_controller) {
    if (gamepad_controller->onLeft()) {
        this->onLeft();
    } else if (gamepad_controller->onRight()) {
        this->onRight();
    }
}

void MainMenuPage::executeKeyboardEvent(KeyboardController *keyboard_controller) {
    if (keyboard_controller->onLeft()) {
        this->onLeft();
    } else if (keyboard_controller->onRight()) {
        this->onRight();
    }
}

void MainMenuPage::executeSpamEvents(GamepadController *gamepad_controller, KeyboardController *keyboard_controller) {
    if (gamepad_controller->onLeftSpam()) {
        this->onLeftSpam();
    } else if (gamepad_controller->onRightSpam()) {
        this->onRightSpam();
    }

    if (keyboard_controller->onLeftSpam()) {
        this->onLeftSpam();
    } else if (keyboard_controller->onRightSpam()) {
        this->onRightSpam();
    }
}

void MainMenuPage::setGameTitleFont() {
    gameTitleFont = TTF_OpenFont(gameFontFamilyName.c_str(), gameTitleFontSize);
    clockTextFont = TTF_OpenFont(clockFontFamilyName.c_str(), clockTextFontSize);

    if (!gameTitleFont || !clockTextFont) {
        std::cerr << "Failed to open font." << '\n';
    }

    gameTitle.setFont(gameTitleFont);
    gameTitle.setColor(SDL_Color{255, 255, 255, 255});
    if (games.size() >= 1) {
        gameTitle.setText(games[selectedGame].name);
    }
    gameTitle.setFontScale(gameTitleFontScale);
    gameTitle.setRenderMethod(Text::RenderMethod::Blended);
    gameTitle.setPosition(Size(gameTitleX, SIZE_WIDTH), Size(gameTitleY, SIZE_HEIGHT));

    clockText.setFont(clockTextFont);
    clockText.setColor(SDL_Color{255, 255, 255, 255});
    clockText.setText(utils::currentDateTime(clockFormat));
    clockText.setFontScale(clockTextFontScale);
    clockText.setRenderMethod(Text::RenderMethod::Blended);
    clockText.setPosition(Size(clockTextX, SIZE_WIDTH), Size(clockTextY, SIZE_HEIGHT));

};

void MainMenuPage::animateGames() {
    for (int i = 0; i < static_cast<int>(gameEntities.size()); i++) {
        int newX, newY, newW, newH;

        newX = (i * (gameSizeNormal + (marginBetweenGames * 2))) -
               selectedGame * (gameSizeNormal + (marginBetweenGames * 2)) + gameOffset;
        newY = normalY;
        newW = gameSizeNormal;
        newH = gameSizeNormal;

        if (i == selectedGame) {
            newW = gameSizeSelected;
            newH = gameSizeSelected;
        }

        if (i > selectedGame) {
            newX += selectedGameOffset;
        }

        gameEntities[i].setAnimation(
                MultiSize(Size(newX, SIZE_HEIGHT), Size(newY, SIZE_HEIGHT), Size(newW, SIZE_HEIGHT),
                          Size(newH, SIZE_HEIGHT)), isSpamming ? spamTransitionTime : normalTransitionTime);
    }
};

void MainMenuPage::createGameEntity(int i) {
    const MultiSize normalGameDims(
            Size(i * (gameSizeNormal + (marginBetweenGames * 2) + selectedGameOffset) + gameOffset, SIZE_HEIGHT),
            Size(normalY, SIZE_HEIGHT), Size(gameSizeNormal, SIZE_HEIGHT), Size(gameSizeNormal, SIZE_HEIGHT));
    const MultiSize selectedGameDims(Size(gameOffset - selectedGameOffset, SIZE_HEIGHT), Size(normalY, SIZE_HEIGHT),
                                     Size(gameSizeSelected, SIZE_HEIGHT), Size(gameSizeSelected, SIZE_HEIGHT));

    Entity anotherGame(i == 0 ? selectedGameDims : normalGameDims, gameBorder);

    gameEntities.push_back(anotherGame);
};

void MainMenuPage::readGameStyles() {
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

    clockFontFamilyName = gameStyles["clock"]["filename"];
    clockTextFontSize = gameStyles["clock"]["size"];
    clockTextFontScale = gameStyles["clock"]["scale"];
    clockTextX = gameStyles["clock"]["x"];
    clockTextY = gameStyles["clock"]["y"];

    gameOffset = gameStyles["game-container"]["x"]["value"];
    marginBetweenGames = gameStyles["game-container"]["spacing"]["value"];

    normalY = gameStyles["game-container"]["y"]["value"];

    normalTransitionTime = gameStyles["game-container"]["normalTransition"];
    spamTransitionTime = gameStyles["game-container"]["spamTransition"];
};

void MainMenuPage::requestSteamGamesFromServer() {
    try {
        auto gamesResponse = api->get("steam");
        std::cout << "Got " << gamesResponse.size() << " games from the server!" << std::endl;

        for (int i = 0; i < static_cast<int>(gamesResponse.size()); i++) {
            this->createGameEntity(i);
            games.push_back({gamesResponse[i]["Name"], gamesResponse[i]["SteamAppID"]});
        }

        this->animateGames();
    }
    catch (const std::exception &e) {
        std::cerr << "Request failed, error: " << e.what() << '\n';
    }
};