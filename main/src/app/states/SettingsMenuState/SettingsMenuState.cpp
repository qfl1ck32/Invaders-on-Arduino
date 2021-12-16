#include "./SettingsMenuState.h"

#include "../../globals.h"

void SettingsMenuState::setup() {
    static const char *const messages[] = {"Back", "Level", "LCD", "Matrix", "Sounds"};

    menu.setMessages(messages, sizeof(messages) / sizeof(char *));

    HandlerFunction handlers[] = {SettingsMenuState::goBack, SettingsMenuState::goToLevel, SettingsMenuState::goToLCD,
                                  SettingsMenuState::goToMatrix, SettingsMenuState::goToSounds};

    menu.setOns(handlers, sizeof(handlers) / sizeof(HandlerFunction));

    joystick.setOnChangeUp(menuGoUp);
    joystick.setOnChangeDown(menuGoDown);

    joystick.setOnSwStateChange(menuSelect);
}

void SettingsMenuState::handle() {
    menu.run();
    joystick.handleJoystickMovements();
}

void SettingsMenuState::cleanup() {
    joystick.clearHandlers();
}

void SettingsMenuState::goBack() {
    stateManager.changeState(mainMenuStateId);
}

void SettingsMenuState::goToLevel() {
    stateManager.changeState(settingsLevelMenuStateId);
}

void SettingsMenuState::goToLCD() {
    stateManager.changeState(settingsLCDMenuStateId);
}

void SettingsMenuState::goToMatrix() {
    stateManager.changeState(settingsMatrixMenuStateId);
}

void SettingsMenuState::goToSounds() {
    stateManager.changeState(settingsSoundsMenuStateId);
}