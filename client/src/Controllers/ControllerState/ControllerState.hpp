#pragma once

#include <SDL2/SDL.h>

#include <iostream>
#include <vector>

class ControllerState {
public:
    void syncState();

    void updateState(int p_keyBind, bool p_newValue);

    void updateKeyboardState(int p_keyboardScanCode, bool p_newValue);

    void updateGamepadState(int p_controllerButton, bool p_newValue);

    bool isDown(int p_keyBind);

    bool isUp(int p_keyBind);

    enum keyBinds {
        enter = 0
    };

private:
    std::vector<bool> state = {false};
    std::vector<bool> previousState = {false};

    const std::vector<int> gamepadKeyBinds{
            SDL_CONTROLLER_BUTTON_A // enter
    };

    const std::vector<int> keyboardKeyBinds{
            SDL_SCANCODE_RETURN // enter
    };
};