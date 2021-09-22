#pragma once

#include <SDL2/SDL.h>
#include <SDL2/SDL_image.h>
#include <SDL2/SDL_ttf.h>

#include "Rendering/RenderWindow/RenderWindow.hpp"
#include "Rendering/Entity/Entity.hpp"
#include "Rendering/Text/Text.hpp"

#include "Networking/Server/Server.hpp"

#include "Utilities/Sound/Sound.h"

#include "Controllers/GamepadController/GamepadController.hpp"
#include "Controllers/KeyboardController/KeyboardController.hpp"

#include <iostream>
#include <vector>

class MainMenuPage {
    public: 
        MainMenuPage(RenderWindow* p_window, Server* p_api);

        void render(double deltaTime);

        void executeKeyboardEvent(KeyboardController* keyboard_controller);
        void executeControllerEvent(GamepadController* gamepad_controller);
        void executeSpamEvents(GamepadController* gamepad_controller, KeyboardController* keyboard_controller);

        void initialize(SDL_Event& event);
        void cleanUp();

        void requestSteamGamesFromServer();
        void createGameEntity(int i, std::string name);
        void animateGames();
        void readGameStyles();
        void setGameTitleFont();

        void onRight();
        void onLeft();
        void onRightSpam();
        void onLeftSpam();

    private:
        RenderWindow* window;
        Entity wallpaperEntity;
        std::vector<Entity> gameEntities = {};
        std::vector<std::string> gameNames = {};
        float gameSizeNormal, gameSizeSelected, selectedGameOffset, gameOffset, marginBetweenGames, normalY, gameTitleFontScale, gameTitleX, gameTitleY;
        int normalTransitionTime, spamTransitionTime, gameTitleFontSize, selectedGame = 0, prevSelectedGame = 0;
        bool isSpamming = false;
        std::string gameFontFamilyName;
        SDL_Texture* gameBorder;
        Server* api;
	    GamepadController gamepadController;
	    KeyboardController keyboardController;
        Text gameTitle, clockText;
        TTF_Font *gameTitleFont = NULL, *clockTextFont = NULL;
        Audio * scrollSound = NULL;
        std::string clockFontFamilyName;
        float clockTextFontSize, clockTextFontScale, clockTextX, clockTextY;
};