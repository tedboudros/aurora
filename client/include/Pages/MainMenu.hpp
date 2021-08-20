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

    private:
        RenderWindow* window;
        Entity wallpaperEntity;
};