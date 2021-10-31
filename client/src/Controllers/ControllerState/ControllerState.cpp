
#include "ControllerState.hpp"
#include "Utilities/Helpers/Helpers.hpp"

void ControllerState::syncState() {
    previousState = state;
}

void ControllerState::updateState(int p_keyBind, bool p_newValue) {
    if (p_keyBind != -1) {
        state[p_keyBind] = p_newValue;
    }
}

void ControllerState::updateKeyboardState(int p_keyboardScanCode, bool p_newValue) {
    int keyBind = utils::getIndex(keyboardKeyBinds, p_keyboardScanCode);
    updateState(keyBind, p_newValue);
}


void ControllerState::updateGamepadState(int p_controllerButton, bool p_newValue) {
    int keyBind = utils::getIndex(gamepadKeyBinds, p_controllerButton);
    updateState(keyBind, p_newValue);
}

bool ControllerState::isDown(int p_keyBind) {
    return state[p_keyBind] && !previousState[p_keyBind];
}

bool ControllerState::isUp(int p_keyBind) {
    return !state[p_keyBind] && previousState[p_keyBind];
}