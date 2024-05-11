#include "buttonHandler.h"

void ButtonHandler::handleLeftButton() {

    if(currentSubState == SubScreenState::NO_STATE) {
        switch(currentState) {
            case ScreenState::ALARM_SCREEN_VIEW: changeStateLeft(); break;
            case ScreenState::WEATHER_SCREEN: changeStateLeft(); break;
            case ScreenState::TEMP_HUMIDITY_SCREEN: changeStateLeft(); break;
            case ScreenState::NEWS_SCREEN: changeStateLeft(); break;
            default: break;
            }
    } else {
        switch (currentSubState) {
            case SubScreenState::SET_ALARM_SCREEN: break;
            case SubScreenState::SET_IP_ADDRESS: break;
            case SubScreenState::NO_STATE: break;
            default: break;
        }
    }
}

void ButtonHandler::handleMiddleButton() {
    switch(currentState) {
        case ScreenState::ALARM_SCREEN_VIEW: alarmScreen.setAlarmTime(); break;
        case ScreenState::WEATHER_SCREEN: break;
        case ScreenState::TEMP_HUMIDITY_SCREEN: break;
        case ScreenState::NEWS_SCREEN: break;
        default: break;
        }
}

void ButtonHandler::handleRightButton() {
    switch(currentState) {
        case ScreenState::ALARM_SCREEN_VIEW: changeStateRight(); break;
        case ScreenState::WEATHER_SCREEN: changeStateRight(); break;
        case ScreenState::TEMP_HUMIDITY_SCREEN: changeStateRight(); break;
        case ScreenState::NEWS_SCREEN: changeStateRight(); break;
        default: break;
        }
}

void ButtonHandler::handleSpecialButton() {
    switch(currentState) {
        case ScreenState::ALARM_SCREEN_VIEW: break;
        case ScreenState::WEATHER_SCREEN: break;
        case ScreenState::TEMP_HUMIDITY_SCREEN: break;
        case ScreenState::NEWS_SCREEN: break;
        default: break;
        }
}

void ButtonHandler::changeStateLeft() {
    screenHandler.changeScreenLeft();
    currentState = static_cast<ScreenState>(screenHandler.getCurrentScreenNumber());
}

void ButtonHandler::changeStateRight() {
    screenHandler.changeScreenRight();
    currentState = static_cast<ScreenState>(screenHandler.getCurrentScreenNumber());
}

void ButtonHandler::changeSubState() {
    screenHandler.changeSubScreen();
    currentSubState = static_cast<SubScreenState>(screenHandler.getCurrentSubScreenNumber());
}

