#pragma once

#include <SDL2/SDL.h>
#include <SDL2/SDL_image.h>

#include "RenderWindow.hpp"
#include "Entity.hpp"

#include <iostream>
#include <vector>

class MainMenuPage {
    public: 
        MainMenuPage(RenderWindow* window);

        void initialize(SDL_Event& event);
        void render(double deltaTime);
        void clear();

        void requestSteamGamesFromServer();
        void createGameEntity(int i, std::string name);
        void readGameStyles();

    private:
        RenderWindow* window;
        Entity wallpaperEntity;
        std::vector<Entity> gameEntities = {};
        std::vector<std::string> gameNames = {};
        float gameSizeNormal, gameSizeSelected, selectedGameOffset, gameOffset, marginBetweenGames, normalY, gameTitleFontScale, gameTitleX, gameTitleY;
        int normalTransitionTime, spamTransitionTime, gameTitleFontSize, server_port;
};