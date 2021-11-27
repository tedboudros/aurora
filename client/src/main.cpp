#include <SDL2/SDL.h>
#include <iostream>
#include <cstdio>

#include "Controllers/ControllerState/ControllerState.hpp"
#include "Controllers/GamepadController/GamepadController.hpp"
#include "Controllers/KeyboardController/KeyboardController.hpp"

#include "Rendering/RenderWindow/RenderWindow.hpp"
#include "Pages/MainMenu/MainMenu.hpp"

#include "Utilities/Helpers/Helpers.hpp"

int main(int argc, char *args[]) {
    bool isRunning = true;

    if (argc <= 1) {
        std::cout << "Missing port argument.\n\nPress any key to exit aurora..." << std::endl;
        std::getchar();
        return 1;
    }

    Server api(args[1]);

    RenderWindow window("Aurora", 1920, 1080);

    MainMenuPage mainMenu(&window, &api);

    // Diagnostic stuff:
    int windowRefreshRate = window.getRefreshRate();
    std::cout << "Refresh rate: " << windowRefreshRate << "hz" << std::endl;

    Vector2f windowSize = window.getWindowDimensions();
    std::cout << "Resolution: " << windowSize.x << "x" << windowSize.y << std::endl;


    SDL_Event event;

    GamepadController gamepadController;
    KeyboardController keyboardController;
    ControllerState controllerState;

    // For delta time
    Uint64 currentTime = utils::hireTime();

    // Game loop:
    while (isRunning) {
        Uint64 newTime = utils::hireTime();
        double deltaTime = ((newTime - currentTime) * 1000 / (double) SDL_GetPerformanceFrequency());
        currentTime = newTime;

        // SDL Events:
        while (SDL_PollEvent(&event)) {
            switch (event.type) {
                case SDL_QUIT:
                    isRunning = false;
                    break;

                case SDL_JOYBUTTONDOWN:
                    gamepadController.handleUpdateControllerState(event, &controllerState);
                    break;

                case SDL_JOYBUTTONUP:
                    gamepadController.handleUpdateControllerState(event, &controllerState);
                    break;

                case SDL_JOYAXISMOTION:
                    gamepadController.handleNavigationEvents(event);
                    mainMenu.executeControllerEvent(&gamepadController);
                    break;

                case SDL_KEYDOWN:
                    keyboardController.handleNavigationEvents(event);
                    keyboardController.handleUpdateControllerState(event, &controllerState);
                    mainMenu.executeKeyboardEvent(&keyboardController);
                    break;

                case SDL_KEYUP:
                    keyboardController.handleNavigationEvents(event);
                    keyboardController.handleUpdateControllerState(event, &controllerState);
                    break;
            }
        }


        // Keyboard & Gamepad:
        mainMenu.executeControllerState(&controllerState);
        controllerState.syncState();

        gamepadController.spamController(deltaTime);
        keyboardController.spamKeyboard(deltaTime);

        mainMenu.executeSpamEvents(&gamepadController, &keyboardController);


        // Mouse:
        int mouseX, mouseY;
        SDL_GetMouseState(&mouseX, &mouseY);
        mainMenu.handleMouseMovement(mouseX, mouseY);


        // Rendering:
        window.clear();
        mainMenu.render(deltaTime);
        window.display();

    }

    SDL_Quit();

    return 0;
}